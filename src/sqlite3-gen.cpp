/*! \file
    \brief [ ] Описание
*/

#include "umba/umba.h"
//
#include "umba/filesys.h"
#include "umba/string_plus.h"
#include "umba/macros.h"
#include "umba/macro_helpers.h"

#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/sort_includes.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <queue>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <random>
#include <cstdlib>
#include <exception>
#include <stdexcept>


struct IsSpace
{
    bool operator()(char ch) const
    {
        return ch==' ' || ch=='\t' || ch=='\r' || ch=='\n';
    }
};

inline
std::string trim(const std::string &str)
{
    return marty_cpp::simple_trim(str, IsSpace());
}

inline
std::string appendWithSep(std::string str, const std::string &sAppend, const std::string &sSep = " ")
{
    if (sAppend.empty()) // Do nothing on empty append
        return str;

    if (str.empty())
    {
        str.append(sAppend);
    }
    else
    {
        str.append(sSep);
        str.append(sAppend);
    }

    return str;
}

class StringAppendWithSep
{
    std::string  strSep;
    std::string  strBuf;

public:

    StringAppendWithSep(const std::string &sep="", const std::string &strOrg=std::string())
    : strSep(sep)
    , strBuf(strOrg)
    {}

    std::string modifySep(std::string newSep)
    {
        std::swap(strSep, newSep);
        return newSep;
    }

    StringAppendWithSep& append(const std::string &s)
    {
        strBuf = appendWithSep(strBuf, s, strSep);
        return *this;
    }

    StringAppendWithSep& append(std::size_t n, char ch)
    {
        append(std::string(n, ch));
        return *this;
    }

    StringAppendWithSep& append(char ch)
    {
        append(1, ch);
        return *this;
    }

    StringAppendWithSep& concat(const std::string &s)
    {
        strBuf.append(s);
        return *this;
    }

    StringAppendWithSep& concat(std::size_t n, char ch)
    {
        strBuf.append(n, ch);
        return *this;
    }

    StringAppendWithSep& concat(char ch)
    {
        strBuf.append(1, ch);
        return *this;
    }

    std::string toString() const
    {
        return strBuf;
    }


};


struct IsPtrSign
{
    bool operator()(char ch) const
    {
        return ch=='*';
    }
};

struct IsRefSign
{
    bool operator()(char ch) const
    {
        return ch=='*';
    }
};


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

const std::unordered_map<std::string,std::string>& getInitialNames()
{
    static
    std::unordered_map<std::string,std::string> m = 
    { {"void**"                , "ppv"}
    , {"void*"                 , "pv"}
    , {"void"                  , ""}
    , {"int"                   , "i"}
    , {"const void*"           , "pcv"}
    , {"double"                , "dbl"}
    , {"char**"                , "ppStr"}
    , {"const char*"           , "pcStr"}
    , {"const unsigned char*"  , "pcBytes"}
    , {"unsigned char*"        , "pBytes"}
    , {"char***"               , "pppStr"}
    , {"int*"                  , "pInt"}
    , {"char*"                 , "pStr"}
    , {"const char**"          , "ppcStr"}
    , {"const void**"          , "ppcv"}
    , {"char const**"          , "ppcStr"}
    , {"char const*"           , "pcStr"}
    , {"unsigned"              , "u"}
    , {"unsigned int"          , "u"}
    , {"va_list"               , "vaLst"}
    , {"char"                  , "ch"}
    , {"sqlite3_blob**"        , "ppBlob"}
    , {"sqlite3_blob*"         , "pBlob"}
    , {"sqlite3_context*"      , "pCtx"}
    , {"sqlite3_stmt*"         , "pStmt"}
    , {"sqlite_int64"          , "i64"}
    , {"sqlite3_int64"         , "i64"}
    , {"sqlite3_int64*"        , "pI64"}
    , {"const sqlite3_value*"  , "pcVal"}
    , {"sqlite3_mutex*"        , "pMutex"}
    , {"sqlite_uint64"         , "u64"}
    , {"sqlite3_uint64"        , "u64"}
    , {"sqlite3_value**"       , "ppVal"}
    , {"sqlite3*"              , "pDb"}
    , {"sqlite3_value*"        , "pVal"}
    , {"const sqlite3_module*" , "pcMod"}
    , {"sqlite3_callback"      , "cbk"}
    , {"sqlite3_str*"          , "pqStr"}
    , {"..."                   , ""}
    , {"sqlite3**"             , "ppDb"}
    , {"sqlite3_stmt**"        , "ppStmt"}
    , {"sqlite3_blob*"         , "pBlob"}
    , {"sqlite3_vfs*"          , "pVfs"}
    , {"sqlite3_backup*"       , "pBckp"}
    , {"sqlite3_index_info*"   , "pIdxInfo"}
    , {"sqlite3_file*"         , "pFile"}
    , {"sqlite3_file"          , "file"}
    , {"DWORD"                 , "dw"}
    , {"LPCWSTR"               , "lpcwStr"}
    , {"LPWSTR"                , "lpwStr"}
    
    //, {"", ""}
    };

    return m;
}

         
         
//           
// std::unordered_set<std::string>& getKnownTypes()
// {
//     static
//     std::unordered_set<std::string> s;
//     return s;
// }



struct FunctionInfo;
struct FunctionArgInfo;


struct FunctionInfo
{
    std::string                     retType;
    std::string                     ptrType;
    std::string                     name   ;
    std::vector<FunctionArgInfo>    args   ;

    bool hasEllipsisArg() const;

};

struct FunctionArgInfo
{
    bool           simple = true;
    std::string    simpleType   ;
    FunctionInfo   fnPointerType;
    std::string    name         ;

    bool isEllipsisType() const
    {
        if (!simple)
            return false;

        return simpleType=="...";
    }
};

inline
bool FunctionInfo::hasEllipsisArg() const
{
    for(const auto &a: args)
    {
        if (a.isEllipsisType())
            return true;
    }

    return false;
}



inline
void clearArgNames(FunctionInfo &fi)
{
    for(auto &arg: fi.args)
    {
        arg.name.clear();
    }
}

inline
void generateArgNames(FunctionInfo &fi)
{
    const std::unordered_map<std::string,std::string>& initialNames = getInitialNames();

    std::unordered_set<std::string>  usedNames;
    for(auto &arg: fi.args)
    {
        if (!arg.name.empty())
            continue;

        std::string candi;
        if (!arg.simple)
        {
            candi = "pfn";
        }
        else
        {
            if (arg.simpleType=="..." || arg.simpleType=="void")
                continue;

            std::unordered_map<std::string,std::string>::const_iterator it = initialNames.find(arg.simpleType);
            if (it!=initialNames.end())
                candi = it->second;
            if (candi.empty())
                candi = "arg";
        }

        std::string finalCandi = candi;
        unsigned i = 1;
        static const char *idxes = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
        while(true)
        {
            if (usedNames.find(finalCandi)==usedNames.end())
            {
                usedNames.insert(finalCandi);
                arg.name = finalCandi;
                break;
            }
            else
            {
                ++i;
                if (i>16) // !!!
                    break;
                finalCandi = candi;
                finalCandi.append(1, idxes[i]);
            }
        }
    }
}

struct FnDefGenerateOptions
{
    std::string prototypePrefix; // EXPORT/IMPORT/etc

    bool        overrideFnType = false;
    std::string fnTypeOverride ; // WINAPI/stdcall/cdecl/etc

}; // struct FnDefGenerateOptions

inline
std::string generateFunctionName( const std::string &name, std::string overrideNameTemplate
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    //umba::macros::StringStringMap<std::string> m;

    macros["FunctionName"] = name;

    if (overrideNameTemplate.empty())
    {
        overrideNameTemplate = "$(FunctionName)";
    }

    return
    umba::macros::substMacros( overrideNameTemplate
                             , umba::macros::MacroTextFromMapRef<std::string>(macros)
                             , umba::macros::smf_KeepUnknownVars | umba::macros::smf_DisableRecursion
                             );
}

inline
std::string generateFunctionName( const FunctionInfo &fi, std::string overrideNameTemplate
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    return generateFunctionName(fi.name, overrideNameTemplate, macros);
}

inline
std::string generateFunctionCall( FunctionInfo fi, std::string overrideNameTemplate, FnDefGenerateOptions fnDefGenerateOptions
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    StringAppendWithSep argBuf = StringAppendWithSep(", ");
    for(auto argInfo: fi.args)
    {
        argBuf.append(argInfo.name);
    }

    //StringAppendWithSep buf = StringAppendWithSep(" ");

    // std::string name = fi.name;
    // if (!overrideName.empty())
    //     name = overrideName;

    return generateFunctionName(fi.name, overrideNameTemplate, macros) + "(" + argBuf.toString() + ")";
}

inline
std::string generateFunctionDef( FunctionInfo fi, bool ptrMode, std::string overrideNameTemplate, FnDefGenerateOptions fnDefGenerateOptions
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                               )
{
    //std::string resStr;

    StringAppendWithSep buf = StringAppendWithSep(" ");

    // std::string name = fi.name;
    // if (!overrideName.empty())
    //     name = overrideName;

    std::string fnType   = fi.ptrType;
    if (fnDefGenerateOptions.overrideFnType)
    {
        fnType = fnDefGenerateOptions.fnTypeOverride;
    }

    if (ptrMode)
    {
        StringAppendWithSep ptrBuf = StringAppendWithSep(" ");
        //resStr = fi.retType + std::string(" (") + fi.ptrType + std::string(" *") + name + std::string(")");
        //buf.append(fi.retType).append("(").concat(fi.ptrType).append('*').concat(name).concat(')');

        ptrBuf.append(fnType).append('*').concat(generateFunctionName(fi, overrideNameTemplate, macros));
        buf.append(fi.retType).append("(").concat(ptrBuf.toString()).concat(')');
    }
    else
    {
        //resStr = fi.ptrType + std::string(" ") + fi.retType + std::string(" ") + name;
        buf.append(fnDefGenerateOptions.prototypePrefix).append(fi.retType).append(fnType).append(generateFunctionName(fi, overrideNameTemplate, macros));
    }

    // parameters

    buf.concat("(");
    //std::string prevSep = buf.modifySep(",")

    StringAppendWithSep argBuf = StringAppendWithSep(", ");

    for(auto argInfo: fi.args)
    {
        std::string fullArg;
        if (argInfo.simple)
        {
            fullArg = argInfo.simpleType;
            if (!argInfo.name.empty())
            {
               fullArg.append(1, ' ');
               fullArg.append(argInfo.name);
            }
        }
        else // not simple
        {
           FunctionInfo argFi = argInfo.fnPointerType;
           clearArgNames(argFi);
           fullArg = generateFunctionDef( argFi, true, argInfo.name, fnDefGenerateOptions);
        
        }

        argBuf.append(fullArg);
    }

    buf.concat(argBuf.toString());
    buf.concat(')');

    return buf.toString();

}

// struct FunctionInfo
// {
//     std::string                     retType;
//     std::string                     ptrType;
//     std::string                     name   ;
//     std::vector<FunctionArgInfo>    args   ;
// };
//  
// struct FunctionArgInfo
// {
//     bool           simple = true;
//     std::string    simpleType   ;
//     FunctionInfo   fnPointerType;
//     std::string    name         ;
// };



bool splitFunctionArgDef( std::string       def
                        , FunctionArgInfo  &argInfo
                        );

bool splitFunctionPointerDef( std::string    def
                            , FunctionInfo  &functionInfo
                            )
{
    std::string::size_type 
    pos = def.find('(');
    if (pos==def.npos)
        return false;

    functionInfo.retType.assign(def, 0, pos);
    functionInfo.retType = normalizeTypeName(functionInfo.retType);

    def.erase(0, pos);
    if (def.empty())
        return false;

    pos = def.find(')');
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

    if (def.back()==';')
        def.erase(def.size()-1);
    def = trim(def);
    if (def.empty())
        return false;
    if (def.front()!='(' || def.back()!=')')
        return false;
    def.erase(def.size()-1);
    def.erase(0, 1);

    pos = ptrTypeAndName.find('*');
    if (pos==def.npos)
        return false;

    functionInfo.ptrType = trim(std::string(ptrTypeAndName, 0, pos));
    functionInfo.name    = trim(std::string(ptrTypeAndName, pos+1));

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
    // const std::unordered_set<std::string>& knownTypes     = getKnownTypes();

    std::string::size_type 
    pos = def.find_last_of("&*");

    if (pos!=def.npos)
    {
        std::string typeStr = std::string(def, 0, pos+1);
        def.erase(0, pos+1);
        return typeStr;
    }

    std::string collectedModifiers;

    //pos = def.find_first_of(' ');
    pos = def.find(' ');

    while(!def.empty() && pos!=def.npos)
    {
        std::string typeStr = trim(std::string(def, 0, pos));
        def.erase(0, pos+1);
        if (!collectedModifiers.empty())
            collectedModifiers.append(1, ' ');
        collectedModifiers.append(typeStr);
        pos = def.find(' ');
    }

    if (!collectedModifiers.empty())
        return collectedModifiers;

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
        bool parseRes = splitFunctionPointerDef( def, argInfo.fnPointerType);
        if (!parseRes)
            return false;

        argInfo.simple     = false;

        // перемещаем имя из инфы функции в инфу аргумента
        argInfo.name = trim(argInfo.fnPointerType.name);
        argInfo.fnPointerType.name.clear();
    }

    return true;
}


inline
void scanForTypes(const FunctionInfo &fnInfo, std::unordered_set<std::string> &foundTypes)
{
    foundTypes.insert(fnInfo.retType);

    for(const auto &arg: fnInfo.args)
    {
        if (arg.simple)
        {
            foundTypes.insert(arg.simpleType);
        }
        else
        {
            scanForTypes(arg.fnPointerType, foundTypes);
        }
    }
}


// struct FunctionInfo
// {
//     std::string                     retType;
//     std::string                     ptrType;
//     std::string                     name   ;
//     std::vector<FunctionArgInfo>    args   ;
// };
//  
// struct FunctionArgInfo
// {
//     bool           simple = true;
//     std::string    simpleType   ;
//     FunctionInfo   fnPointerType;
//     std::string    name         ;
// };



int main( int argc, char* argv[] )
{
    std::string prototypesFilename;
    std::string proxyFunctionsFilename;
    std::string outputType;

    if (argc>1)
    {
        prototypesFilename = argv[1];
    }

    if (argc>2)
    {
        proxyFunctionsFilename = argv[2];
    }

    if (argc>3)
    {
        outputType = argv[3];
    }

    #if defined(WIN32) || defined(_WIN32)
    if (IsDebuggerPresent())
    {
        prototypesFilename     = "../data/sqlite_prototypes.txt";
        proxyFunctionsFilename = "../data/sqlite_proxy_functions_list.txt";
        //outputType             = "proxynumbers";
        //outputType             = "fnptrtable";
        //outputType             = "fnnametable";
        //outputType             = "proxyfns";
        outputType             = "proxybodies";
        outputType             = "proxytypes";
        //outputType             = "";
    }
    #endif


    if (prototypesFilename.empty())
    {
        std::cout << "prototypes filename not taken\n";
        return 1;
    }

    if (proxyFunctionsFilename.empty())
    {
        std::cout << "proxy functions list filename not taken\n";
        return 1;
    }

    if (outputType.empty())
    {
        std::cout << "output type not taken\n";
        return 1;
    }

    

    std::string fileText;
    if (!umba::filesys::readFile(prototypesFilename, fileText))
    {
        std::cout << "failed to read prototypes file\n";
        return 1;
    }
    std::string fileTextNormalizedLf = marty_cpp::normalizeCrLfToLf(fileText);
    std::vector<std::string> lines = marty_cpp::splitToLinesSimple(fileTextNormalizedLf);

    std::unordered_set<std::string> foundTypes;
    std::vector<FunctionInfo> functionInfos;

    std::size_t
    lineNo = 0;
    for(auto line: lines)
    {
        ++lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("/*")) || umba::string_plus::starts_with(line, std::string("//")) || umba::string_plus::starts_with(line, std::string("#")))
        {
            continue; // skip comment lines
        }

        if (line.back()==',')
        {
            std::cout << "error incomplete prototype (possible linefeed) in line " << lineNo << "\n";
            return 1;
        }

        FunctionInfo functionInfo;
        if (!splitFunctionPointerDef(line, functionInfo))
        {
            std::cout << "failed to parse function ptr in line " << lineNo << "\n";
            return 1;
        }

        scanForTypes(functionInfo, foundTypes);

        functionInfos.emplace_back(functionInfo);


        //std::cout << lineNo << ": " << line << "\n";

    }

    #if 0
    std::cout << "\nFound types:" << "\n";
    for(auto foundType: foundTypes)
    {
        std::cout << foundType << "\n";
    }


    std::cout << "\nFunctions:" << "\n";

    FnDefGenerateOptions fnDefGenerateOptions;

    fnDefGenerateOptions.prototypePrefix = "EXPORT";

    fnDefGenerateOptions.overrideFnType  = true;
    fnDefGenerateOptions.fnTypeOverride  = "cdecl";


    for(auto fi: functionInfos)
    {
        // FunctionInfo
        FunctionInfo fiPtr = fi;
        clearArgNames(fiPtr);
        std::string
        generatedFunctionDef = generateFunctionDef(fiPtr, true, "org_sqlite3_$(FunctionName)", fnDefGenerateOptions);
        std::cout << "Fn Ptr  : " << generatedFunctionDef << "\n";

        FunctionInfo fiProto = fi;
        generateArgNames(fiProto);
        generatedFunctionDef = generateFunctionDef(fiProto, false, "sqlite3_$(FunctionName)", fnDefGenerateOptions);
        std::cout << "Fn Proto: " << generatedFunctionDef << "\n";

        generatedFunctionDef = generateFunctionCall(fiProto, "org_sqlite3_$(FunctionName)", fnDefGenerateOptions);
        std::cout << "Fn Call : " << generatedFunctionDef << "\n";

        std::cout << "\n";
    }

    for(auto fi: functionInfos)
    {
       if (fi.hasEllipsisArg())
       {
           std::cout << "Ellipsis function: " << fi.name << "\n";
       }
    }

    #endif


    std::unordered_map<std::string, unsigned> fnDefsByName;
    unsigned idx = (unsigned)-1;
    for(auto fi: functionInfos)
    {
        idx++;
        fnDefsByName[fi.name] = idx;
    }



    std::string proxyFunctionsFileText;
    if (!umba::filesys::readFile(proxyFunctionsFilename, proxyFunctionsFileText))
    {
        std::cout << "failed to read prototypes file\n";
        return 1;
    }
    std::string proxyFunctionsFileTextNormalizedLf       = marty_cpp::normalizeCrLfToLf(proxyFunctionsFileText);
    std::vector<std::string> proxyFunctionsFileTextLines = marty_cpp::splitToLinesSimple(proxyFunctionsFileTextNormalizedLf);

    std::vector<std::string>         proxyFunctionsList;
    std::unordered_set<std::string>  exportedData;

    lineNo = 0;
    for(auto line: proxyFunctionsFileTextLines)
    {
        ++lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("/*")) || umba::string_plus::starts_with(line, std::string("//")) || umba::string_plus::starts_with(line, std::string("#")))
        {
            continue; // skip comment lines
        }

        //bool res
        umba::string_plus::starts_with_and_strip( line, std::string("sqlite3_") );

        std::size_t pos = line.find(' ');
        if (pos==line.npos)
        {
            proxyFunctionsList.emplace_back(line);
        }
        else
        {
            std::string name = trim(std::string(line, 0, pos));
            std::string attr = marty_cpp::toUpper(trim(std::string(line, pos+1)));
            if (attr=="DATA")
            {
                exportedData.insert(name);
            }

            proxyFunctionsList.emplace_back(name);
        }


        

        //std::cout << line << "\n";
    }

    // sqlite3_data_directory DATA
    // sqlite3_temp_directory DATA
    // sqlite3_version DATA

    auto makeFnIdxConstant = [](std::string s)
    {
        return std::string("ORG_SQLITE3_FN_IDX_") + marty_cpp::toUpper(s);
    };

    auto isDataName = [&](std::string s)
    {
        return exportedData.find(s)!=exportedData.end();
    
    };


    // if (outputType=="proxynumbers" || outputType=="fnptrtable" || outputType=="fnnametable")
    {
        if (outputType=="fnnametable")
        {
            std::cout << "const char* orgSqliteFuncNames[" << proxyFunctionsList.size() << "] =\n";

        }
        else if (outputType=="fnptrtable")
        {
            std::cout << "FARPROC orgSqliteFuncPointers[" << proxyFunctionsList.size() << "] = { 0 ";
        }

        unsigned idx = (unsigned)-1;
        for(auto name: proxyFunctionsList)
        {
            idx++;

            if (outputType=="fnnametable"  /* || outputType=="fnptrtable" */ )
            {
                if (!idx)
                    std::cout << "{ ";
                else
                    std::cout << ", ";
            }

            if (outputType=="proxynumbers")
            {
                std::cout << "#define " << makeFnIdxConstant(name) << "        " << idx << "\n";
            }
            else if (outputType=="fnnametable")
            {
                std::cout << "\"sqlite3_" << name << "\"\n";
            }
            else if (outputType=="fnptrtable")
            {
                // std::cout << "0\n";
            }
            else if (outputType=="proxytypes")
            {
                if (isDataName(name))
                {
                    continue;
                }

                std::unordered_map<std::string, unsigned>::const_iterator it = fnDefsByName.find(name);
                if (it==fnDefsByName.end())
                {
                    std::cout << "name '" << name << "' not found\n";
                    std::cerr << "name '" << name << "' not found\n";
                    return 1;
                }

                FnDefGenerateOptions fnDefGenerateOptions;
                //unsigned idx = 
                FunctionInfo fi = functionInfos[it->second];
                clearArgNames(fi);
                std::string
                generatedFunctionDef = generateFunctionDef(fi, true, "sqlite3_$(FunctionName)_fnptr_t", fnDefGenerateOptions);
                std::cout << "typedef " << generatedFunctionDef << ";\n";

            }
            else if (outputType=="proxybodies")
            {
                if (isDataName(name))
                {
                    std::cout << "PROXY_EXPORT " << "PROXY_SQLITE3_" << marty_cpp::toUpper(name) << "_DATA(" << generateFunctionName(name, "sqlite3_$(FunctionName)") <<");\n\n";
                    continue;
                }


                std::unordered_map<std::string, unsigned>::const_iterator it = fnDefsByName.find(name);
                if (it==fnDefsByName.end())
                {
                    std::cout << "name '" << name << "' not found\n";
                    std::cerr << "name '" << name << "' not found\n";
                    return 1;
                }

                FnDefGenerateOptions fnDefGenerateOptions;
                fnDefGenerateOptions.prototypePrefix = "PROXY_EXPORT";
                FunctionInfo fi = functionInfos[it->second];
                generateArgNames(fi);
                FunctionInfo fiClr;
                clearArgNames(fiClr);
                std::string
                generatedFunctionDef = generateFunctionDef(fi, false, "sqlite3_$(FunctionName)", fnDefGenerateOptions);
                std::cout << "" << generatedFunctionDef << "\n";
                std::cout << "{\n";

                std::cout << "    WHATSAPP_TRACE((\"Proxy called: %s\\n\", " << generateFunctionName(fi, "sqlite3_$(FunctionName)") << "));\n";

                if (!fi.hasEllipsisArg())
                {
                    std::cout << "    " << generateFunctionName(fi, "sqlite3_$(FunctionName)_fnptr_t")
                                        << " pfn = getOriginalFunctionPtr<" << generateFunctionName(fi, "sqlite3_$(FunctionName)_fnptr_t") << ">("
                                        << makeFnIdxConstant(fi.name) << ");\n";
                    std::cout << "    " << "return " << generateFunctionCall(fi, "pfn", fnDefGenerateOptions) << ";\n";
                }
                else
                {
                    std::cout << "    PROXY_SQLITE3_" << marty_cpp::toUpper(name) << "_IMPL();\n";
                }

                std::cout << "}\n\n";

            }

            
        }

        if (outputType=="fnnametable" || outputType=="fnptrtable")
        {
            std::cout << "};\n";
        }
    }



    /*
    Ellipsis function: mprintf       - vmprintf
    Ellipsis function: xsnprintf     - xvsnprintf
    Ellipsis function: test_control  - нет va_args версии
    Ellipsis function: db_config     - sqlite3_db_config - нет va_args версии
    Ellipsis function: log           - нет va_args версии
    Ellipsis function: vtab_config   - нет va_args версии
    Ellipsis function: str_appendf   - str_vappendf

    static const sqlite3_api_routines sqlite3Apis = {

    */

    /* TODO: !!!

       1) Надо сгенерировать переменные для указателей на оригинальные функции, установить их в ноль
       2) При вызове прокси функции проверяем указатель на нулевое значение, если ноль - надо инициализировать получив 
          указатель на функцию из оригинальной DLL
       3) Для Ellipsis функций обработчик (наша функция) будет подставляться каким-нибудь макросом на базе имени функции
       4) Прокси-функция тупо шлёт сообщение в лог и вызывает оригинальную версию
    */


    return 0;
}





