/*! \file
    \brief Simple strings normalization - LF normalization, Tab normalization, split to lines
 */

#pragma once


#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
//
#include "marty_cpp.h"
//
#include "marty_enum.h"
//
#include "marty_flags.h"
//
#include "enums.h"
//

#include <cctype>
#include <cstring>
#include <cwctype>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
namespace marty_cpp
{

// enum class ELinefeedType
// {
//     unknown = -1,
//     lf      = 0,
//     cr,
//     crlf,
//     lfcr
//
// };

//----------------------------------------------------------------------------
//! Реализация нормализации различных видов (CR/LF/CRLF/LFCR) переводов строки к универсальному виду - LF
/*! В этих ваших линупсах перенос строки кодируется одним символом 0x0A (\n)
    В ископаемых MacOS используют '\r', но сейчас вроде перешли на линупсовый перевод строки
  */
template<typename StringType> inline
StringType normalizeCrLfToLf(const StringType &str, ELinefeedType *pDetectedLinefeedType = 0)
{
    typedef typename StringType::value_type   char_type;

    // CR LF - 0D 0A
    const char_type CR = (char_type)'\r'; // CR - 0x0D
    const char_type LF = (char_type)'\n'; // LF - 0x0A
    const char_type SP = (char_type)' ' ;

    std::size_t crCount = 0, lfCount = 0;

    std::for_each( str.begin(), str.end()
                 , [&](char_type ch)
                   {
                       if (ch==CR)
                          ++crCount;
                       else if (ch==LF)
                          ++lfCount;
                   }
                 );

    // Тут надо понимать, что предполагается случайное появление символов '\r' среди нормальных '\n', '\r\n' и '\n\r'

    if (crCount==0) // Символов CR нет - возможно есть некоторое количество символов LF - ничего делать не требуется
    {
        if (pDetectedLinefeedType)
           *pDetectedLinefeedType = ELinefeedType::lf;

        return str;
    }

    if (lfCount==0) // Символов LF нет, возможно есть некоторое количество символов CR. CR надо заменить на LF
    {
        if (pDetectedLinefeedType)
           *pDetectedLinefeedType = ELinefeedType::cr;

        StringType resStr; resStr.reserve(str.size());
        std::for_each( str.begin(), str.end(), [&](char_type ch) { if (ch==CR) ch = LF; resStr.append(1,ch); } );
        return resStr;
    }

    if (crCount==lfCount) // CR LF - DOS/Windows перевод строки, надо оставить только LF
    {
        ELinefeedType lfType = ELinefeedType::unknown;
        StringType resStr; resStr.reserve(str.size());
        std::for_each( str.begin(), str.end()
                     , [&](char_type ch)
                         {
                             if ((ch==CR || ch==LF) && lfType==ELinefeedType::unknown)
                             {
                                 if (ch==CR) // первым попался CR - значит, определяем перевод строки как CR LF
                                     lfType = ELinefeedType::crlf;
                                 else        // первым попался LF - значит, определяем перевод строки как LF CR
                                     lfType = ELinefeedType::lfcr;
                             }

                             if (ch!=CR)
                                 resStr.append(1,ch);
                         }
                     );

        if (pDetectedLinefeedType)
           *pDetectedLinefeedType = lfType;

        return resStr;
    }


    // Остался вариант, когда количество символов CR не равно количеству символов LF

    /* CR заменяется на пробел, если предыдущий и следующий - не пробелы и не переводы строки
     */

    if (pDetectedLinefeedType)
       *pDetectedLinefeedType = ELinefeedType::lf;


    StringType resStr;
    bool prevSpace = false;

    typedef typename StringType::const_iterator iterator;

    iterator it    = str.begin();
    iterator itEnd = str.end();

    while(it!=itEnd)
    {
        if (*it!=CR)
        {
            std::size_t spaceLen = (*it==SP) ? 1 : 0;
            prevSpace = spaceLen>0;

            std::size_t symbLen = spaceLen;

            auto itNext = it; std::advance(itNext, 1);
            resStr.append(it, itNext); // добавляем текущий символ
            it = itNext;
        }
        else // found CR
        {
            // nIt - nextIt
            auto nIt = it; ++nIt; // CR - in all encodings - 1 char len

            if (nIt==itEnd)
            {
                // нет следующего символа, текущий - последний - не надо вставлять пробел вместо CR
            }
            else
            {
                std::size_t nextSpaceLen = (*nIt==SP || *nIt==CR || *nIt==LF) ? 1 : 0; // pSymbolTraits->isSpaceOrLinefeedSymbol(nIt, itEnd);
                bool nextSpace = nextSpaceLen>0;
                if (!prevSpace && !nextSpace) // не пробел до и не пробел после - CR заменяется на пробел
                    resStr.append(1, SP);
            }

            // текущий символ не добавляем - это CR, вместо него вставили пробел или нет, и всё

            prevSpace = true;
            ++it; // it += 1 - CR - in all encodings - 1 char len

        }
    }

    return resStr;

}

//----------------------------------------------------------------------------
//! Реализация удаления (нормализация) конечных пробелов (до перевода строки) - на всем тексте, не разбитом на строки
template<typename StringType> inline
StringType stripTextTrailingSpaces(const StringType &str)
{
    typedef typename StringType::value_type     char_type;
    typedef typename StringType::const_iterator iterator;

    const char_type CR = (char_type)'\r'; // CR - 0x0D
    const char_type LF = (char_type)'\n'; // LF - 0x0A
    const char_type SP = (char_type)' ' ;
    const char_type TB = (char_type)'\t' ;

    StringType resStr; resStr.reserve(str.size());
    StringType spacesBuf;

    iterator it    = str.begin();
    iterator itEnd = str.end();

    while(it!=itEnd)
    {
        if (*it==SP || *it==TB)
        {
            // Добавляем пробел в буфер пробелов
            auto itNext = it; ++itNext;
            spacesBuf.append(it, itNext);
            it = itNext;
        }
        else // spLen==0
        {
            // Не пробел

            if (*it==CR || *it==LF) // перевод строки
            {
                spacesBuf.clear(); // Просто удаляем буферизированные пробелы
                auto itNext = it; ++itNext;
                resStr.append(it, itNext);
                it = itNext;
            }
            else // какой-то другой символ
            {
                // Нужно добавить ранее буферизированные пробелы
                resStr.append(spacesBuf);
                spacesBuf.clear();
                // Добавляем найденный символ
                auto itNext = it; ++itNext;
                resStr.append(it, itNext);
                it = itNext;
            }

        } // if (spLen)

    }

    return resStr;

}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType stripLineTrailingSpaces(StringType str)
{
    typedef typename StringType::value_type       char_type;
    typedef typename StringType::reverse_iterator reverse_iterator;
    typedef typename StringType::iterator         iterator;

    // const char_type CR = (char_type)'\r'; // CR - 0x0D
    // const char_type LF = (char_type)'\n'; // LF - 0x0A
    const char_type SP = (char_type)' ' ;
    const char_type TB = (char_type)'\t' ;

    reverse_iterator rit = str.rbegin();
    for(; rit!=str.rend(); ++rit)
    {
        if (*rit==SP || *rit==TB)
            continue;
        break;
    }

    iterator it = rit.base();

    str.erase(it, str.end());

    return str;
}

//----------------------------------------------------------------------------
//! Реализация удаления (нормализация) конечных пробелов (до перевода строки)
template<typename StringType> inline
void stripLineTrailingSpaces(std::vector<StringType> &v)
{
    for(auto &s : v)
    {
        s = stripTrailingSpaces(s);
    }
}

//----------------------------------------------------------------------------
//! Разделяем текст на строки, разделитель строк - простой символ в любой кодировке, по умолчанию - LF (\n)
template<typename StringType> inline
std::vector<StringType> splitToLinesSimple(const StringType &str, bool addEmptyLineAfterLastLf = true, typename StringType::value_type lfChar=(typename StringType::value_type)'\n')
{
    typedef typename StringType::value_type     char_type;
    typedef typename StringType::const_iterator iterator;

    std::vector<StringType> resVec; resVec.reserve(str.size()/32);
    StringType curStr;

    iterator it    = str.begin();
    iterator itEnd = str.end();

    bool prevLf = false;

    for(; it!=itEnd; ++it)
    {
        if (*it==lfChar)
        {
            resVec.emplace_back(curStr);
            curStr.clear();
            prevLf = true;
        }
        else
        {
            curStr.append(1, *it);
            prevLf = false;
        }
    }

    if (!addEmptyLineAfterLastLf)
    {
        prevLf = false;
    }


    if (!curStr.empty() || prevLf)
    {
        resVec.emplace_back(curStr);
    }

    return resVec;

}

//----------------------------------------------------------------------------
/* Нормализация Tab'ов

   Нужна для нормального набора текстов с табами - сорцы, к примеру.
   Или для корректировки отступов.

   Для начала раскрываем все Tab'ы в пробелы согласно правилу замены: 1 Tab -> 4 Spaces (можно задать и 6 и 8)

   Затем заменяем N пробелов на Tab. При этом могут быть обнаружены пробелы, которые ранее затесались среди Tab'ов,
   или в исходном тексте отступы были сделаны пробелами по 4 на левел, но где-то произошел косяк, и был добавлен лишний пробел,
   или, наоборот, пробел был забыт - 3 пробела вместо 4х.

   Тут конечно косяк - если отступы задаются пробелами, и на каждом уровне вложенности было недостаточное количество пробелов -
   то результат "поедет"

   Только ведущие, или все?

   Вход должен быть нормализован по CRLF -> LF

 */

//----------------------------------------------------------------------------
//! Раскрываем табуляцию в пробелы. . Работает для всего текста
template<typename StringType> inline
    StringType expandTabsToSpaces(const StringType &str, std::size_t tabSize=4 )
{
    typedef typename StringType::value_type     char_type;
    typedef typename StringType::const_iterator iterator;

    // const char_type CR = (char_type)'\r'; // CR - 0x0D
    // const char_type LF = (char_type)'\n'; // LF - 0x0A
    const char_type SP = (char_type)' ' ;
    const char_type TB = (char_type)'\t' ;

    StringType resStr; resStr.reserve(str.size());

    for(const auto ch : str)
    {
        if (ch==TB)
        {
            resStr.append(tabSize, SP);
        }
        else
        {
            resStr.append(1, ch);
        }
    }

    return resStr;
}

//----------------------------------------------------------------------------
//! Раскрываем табуляцию в пробелы.
template<typename StringType> inline
void expandTabsToSpaces(std::vector<StringType> &v, std::size_t tabSize=4 )
{
    for(auto &s : v)
    {
        s = expandTabsToSpaces(s, tabSize);
    }
}

//----------------------------------------------------------------------------
//! Заменяем ведущие пробелы на табы. Строка должна быть нормализована по переводам строки - CRLFLF -> LF etc. Работает для всего текста
template<typename StringType> inline
StringType condenseSpacesToTabs(const StringType &str, std::size_t tabSize=4, std::size_t delta=(std::size_t)-1 )
{
    typedef typename StringType::value_type     char_type;
    typedef typename StringType::const_iterator iterator;

    if (tabSize>16)
    {
        tabSize = 16;
    }

    if (tabSize<1)
    {
        tabSize = 1;
    }

    if (delta==(std::size_t)-1)
    {
        delta = tabSize/2;
    }

    if (delta>=tabSize)
    {
        delta = 0;
    }

    const std::size_t restLim = tabSize-delta;

    const char_type CR = (char_type)'\r'; // CR - 0x0D
    const char_type LF = (char_type)'\n'; // LF - 0x0A
    const char_type SP = (char_type)' ' ;
    const char_type TB = (char_type)'\t' ;

    StringType resStr; resStr.reserve(str.size());
    StringType bufferedSpaces;

    // На входе должна быть строка только из пробелов, это не проверяется
    auto condence = [=](const StringType &strSpaces) -> StringType
    {
        std::size_t nTabs       = strSpaces.size() / tabSize;
        std::size_t nRestSpaces = strSpaces.size() % tabSize;
        if (nRestSpaces>=restLim)
        {
            ++nTabs;
        }

        return StringType(nTabs, TB);
    };

    bool leading = true;

    for(const auto ch : str)
    {
        if (leading && ch==SP)
        {
            bufferedSpaces.append(1,ch);
        }
        else
        {
            resStr.append(condence(bufferedSpaces));
            resStr.append(1,ch);
            bufferedSpaces.clear();
            leading = false;

            if (ch==LF)
            {
                leading = true;
            }
        }

    }

    resStr.append(condence(bufferedSpaces));

    return resStr;

}

//----------------------------------------------------------------------------
//! Заменяем ведущие пробелы на табы. Строка должна быть нормализована по переводам строки - CRLFLF -> LF etc
template<typename StringType> inline
void condenseSpacesToTabs(std::vector<StringType> &v, std::size_t tabSize=4, std::size_t delta=(std::size_t)-1 )
{
    for(auto &s : v)
    {
        s = condenseSpacesToTabs(s, tabSize, delta);
    }
}

//----------------------------------------------------------------------------
//! Нормализация отступов - все не кратные заданному размеру табуляции отступы приводятся к кратному размеру
template<typename StringType> inline
StringType normalizeIndents(const StringType &str, std::size_t tabSize=4, std::size_t delta=(std::size_t)-1 )
{
    StringType expanded  = expandTabsToSpaces(str, tabSize);
    StringType condensed = condenseSpacesToTabs(expanded, tabSize, delta);
    return expandTabsToSpaces(condensed, tabSize);
}

//----------------------------------------------------------------------------
//! Нормализация отступов - все не кратные заданному размеру табуляции отступы приводятся к кратному размеру
template<typename StringType> inline
void normalizeIndents(std::vector<StringType> &v, std::size_t tabSize=4, std::size_t delta=(std::size_t)-1 )
{
    for(auto &s : v)
    {
        s = normalizeIndents(s, tabSize, delta);
    }
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template<typename StringType> inline
StringType mergeLines(const std::vector<StringType> &v, ELinefeedType lfType, bool addTrailingNewLine=false)
{
    StringType resText; resText.reserve(v.size()*16);

    StringType lfStr;

    switch(lfType)
    {
        case ELinefeedType::lf             :  lfStr = make_string<StringType>("\n"  ); break;
        case ELinefeedType::cr             :  lfStr = make_string<StringType>("\r"  ); break;
        case ELinefeedType::lfcr           :  lfStr = make_string<StringType>("\n\r"); break;
        case ELinefeedType::linefeedRemove :  lfStr = make_string<StringType>(""    ); break;
        // case ELinefeedType:::  lfStr = make_string<StringType>(""); break;
        default                            :  lfStr = make_string<StringType>("\r\n");
    }

    for (std::size_t i=0; i!=v.size(); ++i)
    {
        if (i)
            resText.append(lfStr);
        resText.append(v[i]);
    }

    if (addTrailingNewLine)
    {
        resText.append(lfStr);
    }

    return resText;
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType converLfToOutputFormat(const StringType &str, ELinefeedType lfType)
{
    typedef typename StringType::value_type     char_type;
    // typedef typename StringType::const_iterator const_iterator;

    const char_type LF = (char_type)'\n'; // LF - 0x0A


    StringType resText; resText.reserve(str.size() + str.size()/32);

    StringType lfStr;

    switch(lfType)
    {
        case ELinefeedType::lf  :  lfStr = make_string<StringType>("\n"); break;
        case ELinefeedType::cr  :  lfStr = make_string<StringType>("\r"); break;
        case ELinefeedType::lfcr:  lfStr = make_string<StringType>("\n\r"); break;
        // case ELinefeedType:::  lfStr = make_string<StringType>(""); break;
        default                 :  lfStr = make_string<StringType>("\r\n");
    }

    for(auto ch: str)
    {
        if (ch==LF)
        {
            resText.append(lfStr);
        }
        else
        {
            resText.append(1, ch);
        }
    }

    return resText;
}



//----------------------------------------------------------------------------

} // namespace marty_cpp


