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
#include "OutputGeneration.h"
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






int main( int argc, char* argv[] )
{

    // Format: entryname[=internal_name|other_module.exported_name]
    //                  [@ordinal [NONAME] ]
    //                  [ [PRIVATE] | [DATA] ]

    #if 0
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


    std::string initialNamesFilename;
    std::string iniFilename;
    std::string prototypesFilename;
    std::string proxyFunctionsFilename;
    std::string outputType;

    if (argc>1)
    {
        initialNamesFilename = argv[1];
    }

    if (argc>2)
    {
        iniFilename = argv[2];
    }

    if (argc>3)
    {
        prototypesFilename = argv[3];
    }

    if (argc>4)
    {
        proxyFunctionsFilename = argv[4];
    }

    if (argc>5)
    {
        outputType = argv[5];
    }


    #if defined(WIN32) || defined(_WIN32)
    if (IsDebuggerPresent())
    {
        initialNamesFilename   = "../data/initial_names.txt";
        iniFilename            = "../proxy.ini";
        prototypesFilename     = "../data/sqlite_prototypes.txt";
        proxyFunctionsFilename = "../data/sqlite_proxy_functions_list.txt";
        //outputType             = "fnindexes";
        //outputType             = "fntables";
        //outputType             = "proxyfns";
        outputType             = "proxytypes";
        outputType             = "ellipsis";
        outputType             = "proxycode";
    }
    #endif

    auto printHelp = [](int res)
    {
        std::cerr << "Usage: gen ARGNAMES.INI CONFIG.INI PROTOTYPES_FILE FNLIST_FILE OUTPUTTYPE\n";
        return res;
    };

    if (initialNamesFilename.empty())
    {
        std::cerr << "config filename not taken\n";
        return printHelp(1);
    }

    if (iniFilename.empty())
    {
        std::cerr << "config filenamenot taken\n";
        return printHelp(1);
    }

    if (prototypesFilename.empty())
    {
        std::cerr << "prototypes filename not taken\n";
        return printHelp(1);
    }

    if (proxyFunctionsFilename.empty())
    {
        std::cerr << "proxy functions list filename not taken\n";
        return printHelp(1);
    }

    if (outputType.empty())
    {
        std::cerr << "output type not taken\n";
        return printHelp(1);
    }



    InputData   inputData;
    ErrInfo     errInfo;
    DllProxyGenerationOptions  proxyGenerationOptions;


    errInfo.fileName = initialNamesFilename;

    std::string initialNamesText;
    if (!umba::filesys::readFile(initialNamesFilename, initialNamesText))
    {
        std::cout << "failed to read initial names file\n";
        return 1;
    }

    if (!parseParamInitialNames(initialNamesText, errInfo, proxyGenerationOptions))
    {
        std::cout << errInfo.errMsg << " in line " << errInfo.lineNo << "\n";
        return 1;
    }


    errInfo.fileName = iniFilename;

    std::string iniText;
    if (!umba::filesys::readFile(iniFilename, iniText))
    {
        std::cout << "failed to read ini file\n";
        return 1;
    }

    if (!parseDllProxyGenerationOptions(iniText, errInfo, proxyGenerationOptions))
    {
        std::cout << errInfo.errMsg << " in line " << errInfo.lineNo << "\n";
        return 1;
    }


    errInfo.fileName = prototypesFilename;

    std::string fileText;
    if (!umba::filesys::readFile(prototypesFilename, fileText))
    {
        std::cout << "failed to read prototypes file\n";
        return 1;
    }

    if (!parsePrototypes(fileText, errInfo, inputData))
    {
        std::cout << errInfo.errMsg << " in line " << errInfo.lineNo << "\n";
        return 1;
    }


    errInfo.fileName = proxyFunctionsFilename;

    std::string proxyFunctionsFileText;
    if (!umba::filesys::readFile(proxyFunctionsFilename, proxyFunctionsFileText))
    {
        std::cout << "failed to read prototypes file\n";
        return 1;
    }

    if (!parseOutputProxyFunctionsList(proxyFunctionsFileText, errInfo, inputData))
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
        generatedFunctionDef = generateFunctionDef(fiPtr, true, "org_$(FunctionName)", fnDefGenerateOptions);
        std::cout << "Fn Ptr  : " << generatedFunctionDef << "\n";

        FunctionInfo fiProto = fi;
        generateArgNames(fiProto);
        generatedFunctionDef = generateFunctionDef(fiProto, false, "$(FunctionName)", fnDefGenerateOptions);
        std::cout << "Fn Proto: " << generatedFunctionDef << "\n";

        generatedFunctionDef = generateFunctionCall(fiProto, "org_$(FunctionName)", fnDefGenerateOptions);
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


    // DllProxyGenerationOptions  proxyGenerationOptions;
    //  
    // proxyGenerationOptions.dllForwardTarget = "e_sqlite3_org";
    // proxyGenerationOptions.dllTarget        = "e_sqlite3"; // .dll";
    //  
    // proxyGenerationOptions.forwardData       = true;
    // proxyGenerationOptions.forwardEllipsis   = true;
    //  

    
    inputData.updateForwards(proxyGenerationOptions);

    if (outputType=="types")
    {
        for(auto foundType: inputData.foundTypes)
        {
            std::cout << foundType << "\n";
        }


        std::vector<std::string> noInitialNameList;
        const auto &paramInitialNames = proxyGenerationOptions.paramInitialNames;

        for(auto foundType: inputData.foundTypes)
        {
            if (paramInitialNames.find(foundType)==paramInitialNames.end())
            {
                // Начальное имя не найдено
                noInitialNameList.emplace_back(foundType);
            }
        }

        if (!noInitialNameList.empty())
        {
            std::cout << "#-------" << "\n";
            for(auto nn: noInitialNameList)
            {
                std::cout << nn << "\n";
            }
        }
    }
    else if (outputType=="ellipsis")
    {
        if (!generateEllipsisReport(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
        }
    }
    else if (outputType=="fntables")
    {
        if (!generateFunctionTables(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
        }
    }
    else if (outputType=="proxytypes")
    {
        if (!generateProxyTypes(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
        }
    }
    else if (outputType=="fnindexes")
    {
        if (!generateProxyIndexes(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
        }
    }
    else if (outputType=="def")
    {
        if (!generateDef(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
        }
    }
    else if (outputType=="proxycode")
    {
        if (!generateProxyCode(std::cout, errInfo, inputData, proxyGenerationOptions))
        {
            std::cout << errInfo.errMsg << "\n";
            std::cerr << errInfo.errMsg << "\n";
            return 1;
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





