/*! \file
    \brief [ ] Описание
*/

#include "umba/umba.h"
//
#include "GenerationOptions.h"
#include "FunctionPrototypeGeneration.h"
#include "FunctionPrototypeTypes.h"
#include "FunctionPrototypeParsing.h"
#include "StringAppendWithSep.h"
#include "InputData.h"
#include "utils.h"

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


// DLL export forwarding
// https://devblogs.microsoft.com/oldnewthing/20121116-00/?p=6073

// Генерация def файла. Имя текущей DLL - задаём константой

// Имя FORWARD DLL - задаём константой

// Создаём список имён, которые нужно отфорвардить (в def-файле), соответственно, код для них не генерируем
// NAME FORWARD
// NAME FORWARD:forwar_dll.dll
// NAME DATA FORWARD
// NAME DATA FORWARD:forwar_dll.dll









// struct IsPtrSign
// {
//     bool operator()(char ch) const
//     {
//         return ch=='*';
//     }
// };
//  
// struct IsRefSign
// {
//     bool operator()(char ch) const
//     {
//         return ch=='*';
//     }
// };





int main( int argc, char* argv[] )
{

    // Format: entryname[=internal_name|other_module.exported_name]
    //                  [@ordinal [NONAME] ]
    //                  [ [PRIVATE] | [DATA] ]

    #if 1
    std::vector<std::string> exports = 
    { "someFunc@ 0"
    , "someFunc = someFunc@de$dwd @3"
    , "someData DATA"
    , "someFunc "
    , "someFunc= someFunc@de$dwd @3"
    , "someFunc=someFunc$de$dwd @3"
    , "someFunc =someFunc$de$dwd @33"
    , "someFunc=someFunc$de$dwd@43"
    , "someFunc=someFunc$de$dwd@ 54"
    , "someFunc@16"
    , "someFunc @ 23"
    , "someFunc=other.a@22 PRIVATE"
    , "someData=other.d@3 DATA"
    , "someData=other.@5 DATA"
    , "someFunc=other.a @22 PRIVATE"
    , "someData=other.d @3 DATA"
    , "someData=other. @5 DATA"
    , "someData=other."
    };

    ModuleExportEntry moduleExportEntry;

    for(auto expLine: exports)
    {
        bool res = parseModuleExportEntry(expLine, moduleExportEntry);
        std::cout << (res?"+ ":"- ") << expLine << "\n";
        if (res)
        {
            std::cout << "        Entry: " << moduleExportEntry.entryName << "\n";
            if (!moduleExportEntry.internalName.empty() && !moduleExportEntry.otherModule.empty())
            {
                std::cout << "        Internal & Forward: error\n";
            }
            else if (!moduleExportEntry.internalName.empty())
            {
                std::cout << "        Internal: " << moduleExportEntry.internalName << "\n";
            }
            else if (!moduleExportEntry.otherModule.empty())
            {
                if (moduleExportEntry.exportedName.empty())
                {
                    std::cout << "        Forward: error\n";
                }
                else
                {
                }
                    std::cout << "        Forward: " << moduleExportEntry.otherModule << "." << moduleExportEntry.exportedName << "\n";
            }

            if (moduleExportEntry.ordinal!=(unsigned)-1)
            {
                std::cout << "        Ordinal: " << moduleExportEntry.ordinal << "\n";
            }

            StringAppendWithSep attrBuf = StringAppendWithSep(" ");
            if (moduleExportEntry.attrs&moduleExportEntry.DefFileFlagNoname)
            {
                attrBuf.append("NONAME");
            }
            if (moduleExportEntry.attrs&moduleExportEntry.DefFileFlagPrivate)
            {
                attrBuf.append("PRIVATE");
            }
            if (moduleExportEntry.attrs&moduleExportEntry.DefFileFlagData)
            {
                attrBuf.append("DATA");
            }

            if (!attrBuf.empty())
            {
               std::cout << "        Attrs: " << attrBuf.toString() << "\n";
            }

        }
    }
    #endif


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
        //outputType             = "fntables";
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

    ErrInfo errInfo;

    errInfo.fileName = prototypesFilename;

    std::unordered_set<std::string>               foundTypes;
    std::vector<FunctionInfo>                     functionInfos;
    std::unordered_map<std::string, std::size_t>  fnDefsByName;

    if (!parsePrototypes(fileText, errInfo, foundTypes, functionInfos, fnDefsByName))
    {
        std::cout << errInfo.errMsg << " in line " << errInfo.lineNo << "\n";
        return 1;
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

    if (outputType=="ellipsis")
    {
        for(auto fi: functionInfos)
        {
           if (fi.hasEllipsisArg())
           {
               std::cout << "Ellipsis function: " << fi.name << "\n";
           }
        }

        return 0;
    }



    // std::unordered_map<std::string, unsigned> fnDefsByName;
    // unsigned idx = (unsigned)-1;
    // for(auto fi: functionInfos)
    // {
    //     idx++;
    //     fnDefsByName[fi.name] = idx;
    // }



    std::string proxyFunctionsFileText;
    if (!umba::filesys::readFile(proxyFunctionsFilename, proxyFunctionsFileText))
    {
        std::cout << "failed to read prototypes file\n";
        return 1;
    }

    std::vector<std::string>         proxyFunctionsList;
    std::unordered_set<std::string>  exportedData;

    errInfo.fileName = proxyFunctionsFilename;
    if (!parseOutputProxyFunctionsList(proxyFunctionsFileText, errInfo, proxyFunctionsList, exportedData))
    {
        std::cout << errInfo.errMsg << " in line " << errInfo.lineNo << "\n";
        return 1;
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


    // if (outputType=="proxynumbers" || outputType=="fnptrtable" || outputType=="fntables")
    {
        if (outputType=="fntables")
        {
            std::cout << "FARPROC orgSqliteFuncPointers[" << proxyFunctionsList.size() << "] = { 0 };\n\n";

            std::cout << "const char* orgSqliteFuncNames[" << proxyFunctionsList.size() << "] =\n";

        }

        unsigned idx = (unsigned)-1;
        for(auto name: proxyFunctionsList)
        {
            idx++;

            if (outputType=="fntables")
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
            else if (outputType=="fntables")
            {
                std::cout << "\"sqlite3_" << name << "\"\n";
            }
            else if (outputType=="proxytypes")
            {
                if (isDataName(name))
                {
                    continue;
                }

                std::unordered_map<std::string, std::size_t>::const_iterator it = fnDefsByName.find(name);
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
                    std::cout << "PROXY_EXPORT " << "PROXY_SQLITE3_" << marty_cpp::toUpper(name) << "_DATA(" /* << generateFunctionName(name, "sqlite3_$(FunctionName)") */ << ");\n\n";
                    continue;
                }


                std::unordered_map<std::string, std::size_t >::const_iterator it = fnDefsByName.find(name);
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

                std::cout << "    WHATSAPP_TRACE((\"Proxy called: %s\\n\", \"" << generateFunctionName(fi, "sqlite3_$(FunctionName)") << "\"));\n";

                if (!fi.hasEllipsisArg())
                {
                    std::cout << "    " << generateFunctionName(fi, "sqlite3_$(FunctionName)_fnptr_t")
                                        << " orgFnPtr = getOriginalFunctionPtr<" << generateFunctionName(fi, "sqlite3_$(FunctionName)_fnptr_t") << ">("
                                        << makeFnIdxConstant(fi.name) << ");\n";
                    std::cout << "    " << (fi.voidReturn() ? "" : "return ") << generateFunctionCall(fi, "orgFnPtr", fnDefGenerateOptions) << ";\n";
                }
                else
                {
                    std::cout << "    PROXY_SQLITE3_" << marty_cpp::toUpper(name) << "_IMPL();\n";
                }

                std::cout << "}\n\n";

            }

            
        }

        if (outputType=="fntables")
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





