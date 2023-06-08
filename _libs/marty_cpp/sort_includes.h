#pragma once

#include "marty_cpp.h"
//
#include "marty_enum.h"
//
#include "marty_enum.h"
//
#include "marty_flags.h"
//

#include <exception>
#include <stdexcept>
#include <unordered_map>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace marty_cpp {



//----------------------------------------------------------------------------
enum class SourceLineType
{
    genericLine, // 0
    emptyLine  , // 1
    includeLine, // 2

    sortOn     , // 3
    sortOff    , // 4

    optSortPush, // 5
    optSortPop , // 6

    optSortUser, // 7
    optSortSystem, // 8

    optSortGrp0 = 16,
    optSortGrp1,
    optSortGrp2,
    optSortGrp3,
    optSortGrp4,
    optSortGrp5,
    optSortGrp6,
    optSortGrp7,

    optSortBlk0 = 32,
    optSortBlk1,
    optSortBlk2,
    optSortBlk3,
    optSortBlk4,
    optSortBlk5,
    optSortBlk6,
    optSortBlk7

};

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace sort_includes_utils {

//------------------------------
// В случае совпадения возвращает длину искомой строки, или 0, если не совпало (частичное совпадение также вернёт 0)
template <typename IteratorType, typename CharType> inline
std::size_t startsWith(IteratorType b, IteratorType e, const CharType *pCmpWith)
{
    std::size_t idx = 0;
    while(pCmpWith[idx]!=0 && b!=e && pCmpWith[idx]==*b)
    {
        ++b;
        ++idx;
    }

    if (pCmpWith[idx]==0) // дошли до конца искомой строки
    {
        return idx;
    }

    return 0;
}

//------------------------------
template <typename IteratorType, typename VectorIteratorType> inline
std::size_t startsWithOneOf(IteratorType b, IteratorType e, VectorIteratorType bValsCmpWith, VectorIteratorType eValsCmpWith)
{
    for(; bValsCmpWith!=eValsCmpWith; ++bValsCmpWith)
    {
        std::size_t len = startsWith(b, e, bValsCmpWith->c_str());
        if (len)
            return len;
    }

    return 0;
}

//------------------------------
template <typename IteratorType> inline
IteratorType eatSpaces(IteratorType b, IteratorType e)
{
    while(b!=e && (*b==' ' || *b=='\t')) ++b;
    return b;
}

//------------------------------
template<typename StringType> inline
StringType normalizeIncludeName(StringType name)
{
    for(std::string::iterator it=name.begin(); it!=name.end(); ++it)
    {
        if (*it==(StringType::value_type)'\\')
            *it = (StringType::value_type)'/';
    }
    return name;
}

//------------------------------
// template<typename StringType> inline
// StringType getIncludeNamePath(const StringType &name)
// {
//      auto sepPos = name.find_last_of((StringType::value_type)'/' /* , name.size() */ );
//      if (sepPos==name.npos)
//          return StringType();
//      return StringType(name,0,sepPos);
// }

//------------------------------
template<typename StringType> inline
std::vector<StringType> make_string_vector(const std::vector<std::string> &strVec)
{
    std::vector<StringType> resVec; resVec.reserve(strVec.size());

    for(const auto &str : strVec)
    {
        resVec.emplace_back(make_string<StringType>(str));
    }

    return resVec;
}

//------------------------------
} // namespace sort_includes_utils

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template <typename StringType, typename IteratorType> inline
SourceLineType parseSourceLineForIncludes(IteratorType b, IteratorType e, StringType *pFoundName = 0)
{
    typedef StringType::value_type char_type;
    using sort_includes_utils::make_string_vector;

    b = sort_includes_utils::eatSpaces(b, e);
    if (b==e)
        return SourceLineType::emptyLine;

    static std::vector<std::string> nosortMarkers   = make_string_vector<StringType>({ "//#-sort", "/*#-sort", "//#nosort", "/*#nosort"});
    static std::vector<std::string>   sortMarkers   = make_string_vector<StringType>({ "//#+sort", "/*#+sort", "//#sort"  , "/*#sort"  });
    static std::vector<std::string>   optSortPush   = make_string_vector<StringType>({ "//#optsortpush", "/*#optsortpush" });
    static std::vector<std::string>   optSortPop    = make_string_vector<StringType>({ "//#optsortpop" , "/*#optsortpop"  });
    static std::vector<std::string>   optSortGrp    = make_string_vector<StringType>({ "//#optsortgrp" , "/*#optsortgrp"  });
    static std::vector<std::string>   optSortBlk    = make_string_vector<StringType>({ "//#optsortblk" , "/*#optsortblk"  });
    static std::vector<std::string>   optSortUser   = make_string_vector<StringType>({ "//#optsortuser", "/*#optsortuser" });
    static std::vector<std::string>   optSortSystem = make_string_vector<StringType>({ "//#optsortsys" , "/*#optsortsys"  });


    std::size_t
    prefixLen = sort_includes_utils::startsWithOneOf(b, e, sortMarkers.begin(), sortMarkers.end());
    if (prefixLen)
        return SourceLineType::sortOn;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, nosortMarkers.begin(), nosortMarkers.end());
    if (prefixLen)
        return SourceLineType::sortOff;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortPush.begin(), optSortPush.end());
    if (prefixLen)
        return SourceLineType::optSortPush;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortPop.begin(), optSortPop.end());
    if (prefixLen)
        return SourceLineType::optSortPop;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortUser.begin(), optSortUser.end());
    if (prefixLen)
        return SourceLineType::optSortUser;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortSystem.begin(), optSortSystem.end());
    if (prefixLen)
        return SourceLineType::optSortSystem;

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortGrp.begin(), optSortGrp.end());
    if (prefixLen)
    {
        SourceLineType optSort = SourceLineType::optSortGrp0;
        b += prefixLen;
        if (b==e)
            return optSort;

        if (*b>=(char_type)'0' && *b<=(char_type)'7')
        {
            optSort = (SourceLineType)(unsigned(optSort) + unsigned(*b-(char_type)'0'));
        }

        return optSort;
    }

    prefixLen = sort_includes_utils::startsWithOneOf(b, e, optSortBlk.begin(), optSortBlk.end());
    if (prefixLen)
    {
        SourceLineType optSort = SourceLineType::optSortBlk0;
        b += prefixLen;
        if (b==e)
            return optSort;

        if (*b>=(char_type)'0' && *b<=(char_type)'7')
        {
            optSort = (SourceLineType)(unsigned(optSort) + unsigned(*b-(char_type)'0'));
        }

        return optSort;
    }


    if (*b!=(char_type)'#')
        return SourceLineType::genericLine;

    ++b;
    if (b==e)
        return SourceLineType::genericLine;

    // Поддержка пробелов между # и include
    b = sort_includes_utils::eatSpaces(b, e);
    if (b==e)
        return SourceLineType::genericLine;

    prefixLen = sort_includes_utils::startsWith(b, e, make_string<StringType>("include").c_str());
    if (!prefixLen)
        return SourceLineType::genericLine;

    b += prefixLen;
    if (b==e)
        return SourceLineType::genericLine;

    b = sort_includes_utils::eatSpaces(b, e);
    if (b==e)
        return SourceLineType::genericLine;

    if (*b!=(char_type)'\"' && *b!=(char_type)'<')
        return SourceLineType::genericLine;

    if (pFoundName)
    {
        StringType tmpName = StringType(b, e);
        *pFoundName = simple_rtrim(tmpName, [](char ch) {return ch==(char_type)' ' || ch==(char_type)'\t' || ch==(char_type)'\r' || ch==(char_type)'\n';} );
    }

    return SourceLineType::includeLine;

}

//----------------------------------------------------------------------------
template<typename StringType> inline
SourceLineType parseSourceLineForIncludes(const StringType &str, StringType *pFoundName = 0)
{
    return parseSourceLineForIncludes<StringType, typename StringType::const_iterator>(str.begin(), str.end(), pFoundName);
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<SourceLineType> parseSourceLinesForIncludes(const std::vector<StringType> &lines, std::unordered_map<std::size_t, StringType> *pNamesByLine = 0)
{
    auto addNameByLine = [&](std::size_t n, const StringType &name)
    {
        if (!pNamesByLine)
            return;

        (*pNamesByLine)[n] = name;
    };

    std::vector<SourceLineType> resSlt; resSlt.reserve(lines.size());

    for(std::size_t n=0; n!=lines.size(); ++n)
    {
        StringType name;
        SourceLineType slt = parseSourceLineForIncludes(lines[n], &name);
        if (slt==SourceLineType::includeLine)
            addNameByLine(n,name);
        resSlt.emplace_back(slt);
    }

    return resSlt;
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
struct SortIncludeBlockInfo
{
    std::size_t      idxBegin;
    std::size_t      idxEnd  ;
    bool             bSort   ;
};

//----------------------------------------------------------------------------
inline
std::vector<unsigned char> findSortIncludeMarkers(const std::vector<SourceLineType> &vecSlt)
{
    std::vector<unsigned char> sortFlags; sortFlags.reserve(vecSlt.size());

    auto backTraceEmptyLines = [&](std::size_t n)
    {
        //std::size_t i = n;
        while(n!=0)
        {
            --n;

            if (sortFlags[n]==0)
                break;

            if (vecSlt[n]==SourceLineType::emptyLine)
            {
                sortFlags[n] = 0;
                continue;
            }

            break;
        }
    };


    int sortLevel = 1; // default is sort

    bool prevInclude = false;

    for(std::size_t n=0; n!=vecSlt.size(); ++n)
    {
        switch(vecSlt[n])
        {
            case SourceLineType::sortOn:
                ++sortLevel;
                sortFlags.emplace_back(0); // do not sort current line with sort mode ctrl
                backTraceEmptyLines(n);
                prevInclude = false;
                break;

            case SourceLineType::sortOff:
                --sortLevel;
                sortFlags.emplace_back(0); // do not sort current line with sort mode ctrl
                backTraceEmptyLines(n);
                prevInclude = false;
                break;

            case SourceLineType::optSortPush:
            case SourceLineType::optSortPop :

            case SourceLineType::optSortUser  :
            case SourceLineType::optSortSystem:

            case SourceLineType::optSortGrp0:
            case SourceLineType::optSortGrp1:
            case SourceLineType::optSortGrp2:
            case SourceLineType::optSortGrp3:
            case SourceLineType::optSortGrp4:
            case SourceLineType::optSortGrp5:
            case SourceLineType::optSortGrp6:
            case SourceLineType::optSortGrp7:

            case SourceLineType::optSortBlk0:
            case SourceLineType::optSortBlk1:
            case SourceLineType::optSortBlk2:
            case SourceLineType::optSortBlk3:
            case SourceLineType::optSortBlk4:
            case SourceLineType::optSortBlk5:
            case SourceLineType::optSortBlk6:
            case SourceLineType::optSortBlk7:
                sortFlags.emplace_back(0); // do not sort current line with sort mode ctrl
                backTraceEmptyLines(n);
                prevInclude = false;
                break;

            case SourceLineType::genericLine:
                sortFlags.emplace_back(0); // do not sort generic lines
                backTraceEmptyLines(n);
                prevInclude = false;
                break;

            case SourceLineType::emptyLine:
                if (prevInclude)
                {
                    sortFlags.emplace_back(sortLevel>0 ? 1 : 0);
                }
                else
                {
                    sortFlags.emplace_back(0);
                }
                break;

            case SourceLineType::includeLine:
                sortFlags.emplace_back(sortLevel>0 ? 1 : 0);
                prevInclude = true;
                break;

            default:
                throw std::runtime_error("marty_cpp::findSortIncludeBlocks: default hits");

        } // switch

    } // for

    return sortFlags;

}

//----------------------------------------------------------------------------
struct SortIncludeOptions
{
    std::size_t sysIncludesFirst  = false;
    std::size_t sepGroupLines     = 1;
    std::size_t sepBlockLines     = 1; // 2;
};

//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> sortIncludes( const std::vector<StringType>     &lines
                                    , const std::vector<unsigned char>  &sortMarkers
                                    , const std::vector<SourceLineType> &vecSlt
                                    , const std::unordered_map<std::size_t, StringType> &namesByLine
                                    , const SortIncludeOptions          &sortOptionsIn = SortIncludeOptions()
                                    )
{
    typedef typename StringType::value_type  char_type;

    if (lines.size()!=sortMarkers.size() || lines.size()!=vecSlt.size())
    {
        throw std::runtime_error("marty_cpp::sortIncludes: size mismatch");
    }

    std::vector<SortIncludeOptions> sortOptions;
    sortOptions.emplace_back(sortOptionsIn);


    std::vector<StringType> resLines; resLines.reserve(lines.size());

    auto skipNonSort = [&](std::size_t n)
    {
        while(n!=sortMarkers.size() && sortMarkers[n]==0)
        {
            resLines.emplace_back(lines[n]);

            switch(vecSlt[n])
            {
                case SourceLineType::optSortPush:
                    sortOptions.push_back(sortOptions.back());
                    break;

                case SourceLineType::optSortPop :
                    if (sortOptions.size()>1)
                        sortOptions.pop_back();
                    break;

                case SourceLineType::optSortUser  :
                    sortOptions.back().sysIncludesFirst = false;
                    break;

                case SourceLineType::optSortSystem:
                    sortOptions.back().sysIncludesFirst = true;
                    break;

                case SourceLineType::optSortGrp0:
                case SourceLineType::optSortGrp1:
                case SourceLineType::optSortGrp2:
                case SourceLineType::optSortGrp3:
                case SourceLineType::optSortGrp4:
                case SourceLineType::optSortGrp5:
                case SourceLineType::optSortGrp6:
                case SourceLineType::optSortGrp7:
                    sortOptions.back().sepGroupLines = std::size_t(vecSlt[n]) - std::size_t(SourceLineType::optSortGrp0);
                    break;

                case SourceLineType::optSortBlk0:
                case SourceLineType::optSortBlk1:
                case SourceLineType::optSortBlk2:
                case SourceLineType::optSortBlk3:
                case SourceLineType::optSortBlk4:
                case SourceLineType::optSortBlk5:
                case SourceLineType::optSortBlk6:
                case SourceLineType::optSortBlk7:
                    sortOptions.back().sepBlockLines = std::size_t(vecSlt[n]) - std::size_t(SourceLineType::optSortBlk0);
                    break;
            }

            ++n;
        }
        return n;
    };

    auto formatIncludes = [&](const std::map<StringType, std::size_t, IncludeNameLess<StringType> > &includesMap)
    {
        bool first = true;
        StringType curGroup;

        std::map<StringType, std::size_t, IncludeNameLess<StringType> >::const_iterator it = includesMap.begin();
        for(; it!=includesMap.end(); ++it)
        {
            if (it->second>=lines.size())
                continue;

            StringType grpName =  /* sort_includes_utils:: */ getIncludeNamePath(it->first);
            if (first)
            {
                curGroup = grpName;
                first = false;
                resLines.emplace_back(lines[it->second]);
                continue;
            }

            if (curGroup!=grpName)
            {
                curGroup = grpName;
                resLines.insert(resLines.end(), sortOptions.back().sepGroupLines, std::string());
            }

            resLines.emplace_back(lines[it->second]);
        }
    };


    for( std::size_t n=skipNonSort(0)
       ; n!=sortMarkers.size()
       ; n=skipNonSort(n)
       )
    {
        std::map<StringType, std::size_t, IncludeNameLess<StringType> >    sysIncludesMap;
        std::map<StringType, std::size_t, IncludeNameLess<StringType> >    userIncludesMap;
        std::set<StringType, IncludeNameLess<StringType> >                 processedIncludes;

        for(; n!=sortMarkers.size() && sortMarkers[n]!=0; ++n )
        {
            if (vecSlt[n]!=SourceLineType::includeLine)
                continue;

            std::unordered_map<std::size_t, StringType>::const_iterator nblIt = namesByLine.find(n);
            if (nblIt==namesByLine.end())
                continue;

            StringType fileName = nblIt->second;

            bool sysInclude = unquote(fileName, (char_type)'<', (char_type)'>');
            if (!sysInclude)
                unquote(fileName, (char_type)'\"', (char_type)'\"');

            if (fileName.empty())
                continue;

            fileName = sort_includes_utils::normalizeIncludeName(fileName);

            if (processedIncludes.find(fileName)!=processedIncludes.end())
                continue;

            processedIncludes.insert(fileName);

            if (sysInclude)
                sysIncludesMap[fileName]  = n;
            else
                userIncludesMap[fileName] = n;

        } // for

        if (sysIncludesMap.empty() && userIncludesMap.empty())
            continue;

        if (sortOptions.back().sysIncludesFirst)
        {
            formatIncludes(sysIncludesMap);
            if (!sysIncludesMap.empty() && !userIncludesMap.empty())
                resLines.insert(resLines.end(), sortOptions.back().sepBlockLines, StringType());
            formatIncludes(userIncludesMap);
        }
        else
        {
            formatIncludes(userIncludesMap);
            if (!sysIncludesMap.empty() && !userIncludesMap.empty())
                resLines.insert(resLines.end(), sortOptions.back().sepBlockLines, StringType());
            formatIncludes(sysIncludesMap);
        }

    } // for

    return resLines;
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> sortIncludes( const std::vector<StringType>    &lines
                                    , const SortIncludeOptions          &sortOptions = SortIncludeOptions()
                                    )
{
    std::unordered_map<std::size_t, StringType> incNames;
    std::vector<SourceLineType> vecSlt  = parseSourceLinesForIncludes(lines, &incNames);
    std::vector<unsigned char>  markers = findSortIncludeMarkers(vecSlt);
    return sortIncludes(lines, markers, vecSlt, incNames, sortOptions);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace marty_cpp

