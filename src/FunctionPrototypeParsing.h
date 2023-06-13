#pragma once

#include "umba/umba.h"
//

#include "FunctionPrototypeTypes.h"
#include "StringAppendWithSep.h"
#include "umba/string_plus.h"
#include "utils.h"

#include <string>
#include <vector>
#include <unordered_set>


inline
std::string normalizeTypeName( std::string str )
{
    std::string strRefPtr;

    while(!str.empty())
    {
        if (str.back()=='*' || str.back()=='&')
        {
            strRefPtr.append(1, str.back());
            str.erase(str.size()-1);
        }
        else if (str.back()==' ')
        {
            str.erase(str.size()-1);
        }
        else
        {
            break;
        }
    }

    std::reverse(strRefPtr.begin(), strRefPtr.end());

    return str + strRefPtr;
}



const std::unordered_set<std::string>& getTypeModifiers()
{
    static
    std::unordered_set<std::string> s; // = {"const", "unsigned", "signed", "long", "short"};
    return s;
}

         
         
//           
std::unordered_set<std::string>& getKnownTypes()
{
    static
    std::unordered_set<std::string> s = 
    { "int"
    , "char"
    };
    return s;
}



bool splitFunctionArgDef( std::string       def
                        , FunctionArgInfo  &argInfo
                        );

bool splitFunctionPointerDef( std::string    def
                            , FunctionInfo  &functionInfo
                            , bool           forcePtrMode = false
                            )
{
    std::string::size_type pos = def.npos;

    // 1)  void * (*sqlite3_aggregate_context)(sqlite3_context*,int nBytes);
    // 2)  void * sqlite3_aggregate_context(sqlite3_context*,int nBytes);

    // В форме указателя можно указывать calling convention - оно само найдётся:
    // void (WINAPI *Sleep)(...)

    // В виде прототипа это выглядит так:
    // void WINAPI Sleep(...)
    // WINADVAPI LSTATUS APIENTRY RegOpenCurrentUser(__in REGSAM samDesired, __out PHKEY phkResult);
    // #define WINAPI      __stdcall
    // #define APIENTRY    WINAPI
    // #define WINADVAPI DECLSPEC_IMPORT

    // И в этом случае нам надо знать, что WINAPI или какое-либо другое аналогичное ключевое слово/макрос - обозначает тип (вызова) функции
    // Это надо как-то задавать снаружи. Пока не будем париться, если надо задать тип конкретной функции - то делаем через форму с указателем

    // Надо глобально задавать тип функций - для тех, где не задано, или для всех (потом)
    // Пока не паримся, надо будет - доделаю


    int prototypeMode = 0; // -1 - fn ptr (*name)(...);  0 - unknown; 1 - fn prototype name(...);

    if (forcePtrMode)
    {
        // Only function pounters allowed
        prototypeMode = -1;
    }
    else
    {
        int      braceLevel   = 0;
        unsigned bracePairCnt = 0;
        for(auto ch: def)
        {
            if (ch=='(')
            {
                braceLevel++;
                //if (braceLevel==0)
            }
            else if (ch==')')
            {
                braceLevel--;
                if (braceLevel==0)
                    bracePairCnt++;
            }
        }

        switch(bracePairCnt)
        {
            case 0:
                prototypeMode = 0; // unknown - ни одной пары скобок
                break;

            case 1:
                prototypeMode = 1; // fn prototype - только одна пара скобок - определение в виде прототипа функции
                break;
        
            case 2:
                prototypeMode = -1; // fn ptr - две пары скобок
                break;
        
            default: // 3+
                prototypeMode = 0; // unknown
        }
    }

    if (!prototypeMode)
    {
        return false;
    }

    if (prototypeMode>0) // fn prototype
    {
        pos = def.find('('); // Ищем открывающую скобку - после неё идут аргументы
        if (pos==def.npos)
            return false;

        std::string retTypeAndFnName = trim(std::string(def, 0, pos)); // До открывающей скобки - тип возвращаемого значения и имя функции
        def.erase(0, pos);

        pos = retTypeAndFnName.find_last_of("&*");

        if (pos!=retTypeAndFnName.npos)
        {
            retTypeAndFnName.insert(pos+1, 1, ' ');
        }

        std::vector<std::string> v;
        pos = retTypeAndFnName.find(' ');
        while(pos!=retTypeAndFnName.npos)
        {
            v.emplace_back(trim(std::string(retTypeAndFnName, 0, pos)));
            retTypeAndFnName.erase(0, pos+1);
            retTypeAndFnName = trim(retTypeAndFnName);
            pos = retTypeAndFnName.find(' ');
        }

        StringAppendWithSep sbRetType = StringAppendWithSep(" ");
        for(const auto vv: v)
        {
            sbRetType.append(vv);
        }    

        functionInfo.retType = normalizeTypeName(sbRetType.toString());
        functionInfo.ptrType = std::string();
        functionInfo.name    = retTypeAndFnName;

        // Разобрали прототип функции
    }
    else // fn ptr
    {
        pos = def.find('('); // Ищем открывающую скобку - в них заключен указатель на функцию
        if (pos==def.npos)
            return false;
    
        functionInfo.retType.assign(def, 0, pos); // До открывающей скобки - тип возвращаемого значения
        functionInfo.retType = normalizeTypeName(functionInfo.retType);
    
        def.erase(0, pos);
        if (def.empty())
            return false;
    
        pos = def.find(')'); // ищем закрывающую скобку указателя на функцию
        if (pos==def.npos)
            return false;
    
        std::string ptrTypeAndName = std::string(def, 0, pos+1);
        def.erase(0, pos+1);
        if (def.empty())
            return false;
        if (ptrTypeAndName.empty())
            return false;
        if (ptrTypeAndName.front()!='(' || ptrTypeAndName.back()!=')')
            return false;
    
        ptrTypeAndName.erase(ptrTypeAndName.size()-1);
        ptrTypeAndName.erase(0, 1);
        ptrTypeAndName = trim(ptrTypeAndName);
        if (ptrTypeAndName.empty())
            return false;

        pos = ptrTypeAndName.find('*');
        if (pos==def.npos)
            return false;
    
        functionInfo.ptrType = trim(std::string(ptrTypeAndName, 0, pos));
        functionInfo.name    = trim(std::string(ptrTypeAndName, pos+1));
            
        // Разобрали имя функции (в форме указателя)
        // 
    }


    if (def.back()==';')
        def.erase(def.size()-1);
    def = trim(def);
    if (def.empty())
        return false;
    if (def.front()!='(' || def.back()!=')')
        return false;
    def.erase(def.size()-1);
    def.erase(0, 1);

    static std::string braces = "(<[";
    std::vector<std::string> argStrings;
    umba::string_plus::ascii_brace::split_against_braces( argStrings  //!< [out] Выхлоп
                                                        , def         //!< Входная строка
                                                        , braces      //!< Обрабатываемые скобки
                                                        , std::string(",")         //!< Разделитель
                                                        , std::string("") // fileName    //!< Имя файла?
                                                        , (size_t)-1  // lineNumber  //!< Номер строки
                                                        , (size_t)-1  //  linePos    //!< Позиция в строке
                                                        , false       // strictOrder //!< Строгий порядок?
                                                        );

    for(auto argStr: argStrings)
    {
        FunctionArgInfo argInfo;
        if (!splitFunctionArgDef(argStr, argInfo))
            return false;
        functionInfo.args.emplace_back(argInfo);
    }

    return true;
}


inline
std::string readType(std::string &def)
{
    // const std::unordered_set<std::string>& knownModifiers = getTypeModifiers();
    const std::unordered_set<std::string>& knownTypes     = getKnownTypes();

    auto isKnownType = [&](const std::string &name)
    {
        return knownTypes.find(name)!=knownTypes.end();
    };

    std::string::size_type 
    pos = def.find_last_of("&*");

    if (pos!=def.npos)
    {
        std::string typeStr = std::string(def, 0, pos+1);
        def.erase(0, pos+1);
        return typeStr;
    }

    // std::string collectedModifiers;
    StringAppendWithSep collectedType = StringAppendWithSep(" ");

    //pos = def.find_first_of(' ');
    pos = def.find(' ');

    while(!def.empty() && pos!=def.npos)
    {
        std::string typeStr = trim(std::string(def, 0, pos));
        def.erase(0, pos+1);
        // if (!collectedModifiers.empty())
        //     collectedModifiers.append(1, ' ');
        collectedType.append(typeStr);
        def = trim(def);
        pos = def.find(' ');
    }

    if (!def.empty())
    {
        if (isKnownType(def))
        {
            collectedType.append(def);
            def.clear();
        }
    }

    if (!collectedType.empty())
        return collectedType.toString();

    std::string res = def;
    def.clear();

    return res;
}


inline
bool splitFunctionArgDef( std::string       def
                        , FunctionArgInfo  &argInfo
                        )
{
    def = trim(def);
    if (def.empty())
        return false;

    std::string::size_type 
    pos = def.find('(');

    if (pos==def.npos) // simple
    {
        std::string type   = trim(readType(def));
        argInfo.simple     = true;
        argInfo.simpleType = normalizeTypeName(type);
        argInfo.name       = trim(def);

        // pos = def.find_last_of(' ');
        // if (pos==def.npos)
        // {
        //     // only type
        //     argInfo.simple     = true;
        //     argInfo.simpleType = normalizeTypeName(def);
        //     // argInfo.fnPointerType
        //     //argInfo.name
        // }
        // else // simple type and name
        // {
        //     argInfo.simple     = true;
        //     argInfo.simpleType = normalizeTypeName(std::string(def, 0, pos));
        //     argInfo.name       = trim(std::string(def, pos+1));
        // }
    }
    else // function ptr
    {
        //FunctionInfo  functionInfo;
        bool parseRes = splitFunctionPointerDef( def, argInfo.fnPointerType, true /* forcePtrMode */ );
        if (!parseRes)
            return false;

        argInfo.simple     = false;

        // перемещаем имя из инфы функции в инфу аргумента
        argInfo.name = trim(argInfo.fnPointerType.name);
        argInfo.fnPointerType.name.clear();
    }

    return true;
}
