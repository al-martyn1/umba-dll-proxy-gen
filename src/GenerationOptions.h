#pragma once

#include "utils.h"
#include "marty_cpp/marty_cpp.h"

#include "ErrInfo.h"
#include "FunctionPrototypeParsing.h"

#include <string>

//! Глобальные опции генерации кода для прокси DLL
struct DllProxyGenerationOptions
{
    bool        configDefOnlyForActiveOption = false  ; //!< #define CONFIG_**OPT** 0/1 или просто #define CONFIG_**OPT** для включенных опций
    std::string configDefPrefix                       ; //!< Префикс для генерируемых макросов условной компиляции с опциями генерации

    std::string dllTarget                             ; //!< this dll name, no ext
    std::string dllForwardTarget                      ; //!< forward target, no ext

    bool forwardData                = false           ; //!< форвардим все данные
    bool forwardEllipsis            = false           ; //!< форвардим все функции с элипсисом

    std::string functionIndexConstantNameFormat       ; //!< Формат константы индекса функции
    std::string ellipsisImplFormat                    ; //!< Формат вызова оригинальной функции (или какой-то замены) для функций с переменным числом аргументов
    std::string functionPtrTypeFormat                 ; //!< Формат указателя на функцию
    std::string getOriginalFunctionPtrFuncTemplateName;
    std::string proxyFunctionImplementationPrefix     ; //!< Something as APIEXPORT etc, or keep it empty
    std::string proxyDataFormat                       ; //!< Если DATA элементы не форвардятся в оригинальную DLL, то они реализовываются по этому шаблону
    std::string proxyImplArraysOrgNamePrefix          ; //!< Префикс для orgFuncPointers/orgFuncNames etc

    std::string proxyHelo                             ; //!< Код с макросами для генерации proxyHelo - однострочный
    bool generateProxyHelo          = false           ; //!< Генерировать код "Proxy called"

    // Пока не реализовано
    std::string customHandler                         ; //!<
    bool generateCustomHandler      = false           ; //!< Генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif

    // Пока не реализовано
    bool generateJumpEllipsis       = false           ; //!< Генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
    bool generateJumpAll            = false           ; //!< Генерим JUMP вместо вызова оригинальной функции для всех прокси


    //! Имена (начальные) для различных типов параметров функций. 
    /*! Используется для генерации имен параметров, если они не заданы в прототипе. 
        К одинаковым именам добавляется номер при дублировании - lpcStr, lpcStr2, lpcStr3, ... 
    */
    std::unordered_map<std::string,std::string> paramInitialNames;

}; // struct DllProxyGenerationOptions


// За неимением гербовой пишем на простой
// Нет полноценного разбора параметров ком строки - парсим инишку с параметрами

//! Разбор INI с именами (начальными) параметров функций
inline
bool parseParamInitialNames( const std::string                &functionsListText
                           , ErrInfo                          &errInfo
                           , DllProxyGenerationOptions        &pgo
                           )
{
    std::string proxyFunctionsFileTextNormalizedLf       = marty_cpp::normalizeCrLfToLf(functionsListText);
    std::vector<std::string> proxyFunctionsFileTextLines = marty_cpp::splitToLinesSimple(proxyFunctionsFileTextNormalizedLf);

    errInfo.lineNo = 0;
    for(auto line: proxyFunctionsFileTextLines)
    {
        ++errInfo.lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("#")) || umba::string_plus::starts_with(line, std::string(";")))
        {
            continue; // skip comment lines
        }

        std::size_t pos    = line.find('=');
        if (pos==line.npos)
        {
            errInfo.errMsg = "missing value";
            return false;
        }

        std::string name  = trim(std::string(line, 0, pos));
        std::string value = trim(std::string(line, pos+1));

        // Надо проверить value на наличие пробелов

        name = normalizeTypeName(name);

        // А надо ли запрещать перезапись?
        #if 0
        std::unordered_map<std::string,std::string>::const_iterator pit = pgo.paramInitialNames.find(name);
        if (pit==pgo.paramInitialNames.end())
        {
            errInfo.errMsg = "initial name for '" + name + "' already defined";
            return false;
        }
        #endif

        pgo.paramInitialNames[name] = value;
    }

    return true;

}

// За неимением гербовой пишем на простой
// Нет полноценного разбора параметров ком строки - парсим инишку с параметрами

// Примеры автогенерации имён параметров:
// int sqlite3_autovacuum_pages(sqlite3* pDb, unsigned int (*pfn)(void*, const char*, unsigned int, unsigned int, unsigned int), void* pv, void (*pfn2)(void*))
// int sqlite3_backup_finish(sqlite3_backup* pBckp)
// sqlite3_backup* sqlite3_backup_init(sqlite3* pDb, const char* pcStr, sqlite3* pDb2, const char* pcStr2)

//! Разбор INI с глобальными опциями генерации кода для прокси DLL
inline
bool parseDllProxyGenerationOptions( const std::string                &functionsListText
                                   , ErrInfo                          &errInfo
                                   , DllProxyGenerationOptions        &pgo
                                   )
{
    std::string proxyFunctionsFileTextNormalizedLf       = marty_cpp::normalizeCrLfToLf(functionsListText);
    std::vector<std::string> proxyFunctionsFileTextLines = marty_cpp::splitToLinesSimple(proxyFunctionsFileTextNormalizedLf);

    errInfo.lineNo = 0;
    for(auto line: proxyFunctionsFileTextLines)
    {
        ++errInfo.lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("#")) || umba::string_plus::starts_with(line, std::string(";")))
        {
            continue; // skip comment lines
        }

        std::size_t posColon = line.find(':');
        std::size_t posEq    = line.find('=');
        std::size_t pos      = posColon<posEq ? posColon : posEq;
        if (pos==line.npos)
        {
            errInfo.errMsg = "missing value";
            return false;
        }

        std::string orgName  = trim(std::string(line, 0, pos));
        std::string name  = marty_cpp::toUpper(orgName);
        std::string value = trim(std::string(line, pos+1));

        auto fromBool = [&](std::string s, bool &res)
        {
            s = marty_cpp::toUpper(s);
            if (s=="TRUE" || s=="+" || s=="YES" || s=="Y" || s=="1")
            {
                res  = true;
                return true; // valid
            }
            if (s=="FALSE" || s=="-" || s=="NO" || s=="N" || s=="0")
            {
                res  = false;
                return true ; // valid
            }

            return false; // not valid
        };

        auto setInvalidMsg = [&](std::string n)
        {
            errInfo.errMsg = "invalid value for parameter '" + n + "'";
            return false;
        };

        auto setUnknownMsg = [&](std::string n)
        {
            errInfo.errMsg = "unknown parameter '" + n + "'";
            return false;
        };

        errInfo.errMsg = "invalid value ";

        
        bool boolVal = false;


        if (name=="CONFIGDEFPREFIX") // ConfigDefPrefix
        {
            pgo.configDefPrefix = value;
            continue;
        }
        if (name=="CONFIGDEFONLYFORACTIVEOPTION")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("ConfigDefOnlyForActiveOption");
            }

            pgo.configDefOnlyForActiveOption = boolVal;
            continue;
        }

        if (name=="DLL" || name=="TARGET" || name=="TARGETDLL") // TargetDll
        {
            pgo.dllTarget = value;
            continue;
        }

        if (name=="FORWARDTARGET") // ForwardTarget
        {
            pgo.dllForwardTarget = value;
            continue;
        }

        if (name=="FORWARDDATA")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("ForwardData");
            }

            pgo.forwardData = boolVal;
            continue;
        }

        if (name=="FORWARDELLIPSIS")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("ForwardEllipsis");
            }

            pgo.forwardEllipsis = boolVal;
            continue;
        }

        if (name=="FUNCTIONINDEXCONSTANTNAMEFORMAT") // FunctionIndexConstantNameFormat
        {
            pgo.functionIndexConstantNameFormat = value;
            continue;
        }

        if (name=="ELLIPSISIMPLFORMAT") // EllipsisImplFormat
        {
            pgo.ellipsisImplFormat = value;
            continue;
        }

        if (name=="FUNCTIONPTRTYPEFORMAT") // FunctionPtrTypeFormat
        {
            pgo.functionPtrTypeFormat = value;
            continue;
        }

        if (name=="GETORIGINALFUNCTIONPTRFUNCTEMPLATENAME") // GetOriginalFunctionPtrFuncTemplateName
        {
            pgo.getOriginalFunctionPtrFuncTemplateName = value;
            continue;
        }

        if (name=="PROXYFUNCTIONIMPLEMENTATIONPREFIX") // ProxyFunctionImplementationPrefix
        {
            pgo.proxyFunctionImplementationPrefix = value;
            continue;
        }

        if (name=="PROXYDATAFORMAT") // ProxyDataFormat
        {
            pgo.proxyDataFormat = value;
            continue;
        }

        if (name=="PROXYIMPLARRAYSORGNAMEPREFIX") // ProxyImplArraysOrgNamePrefix
        {
            pgo.proxyImplArraysOrgNamePrefix = value;
            continue;
        }




        if (name=="PROXYHELO") // ProxyHelo
        {
            pgo.proxyHelo = value;
            continue;
        }
        if (name=="GENERATEPROXYHELO")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("GenerateProxyHelo");
            }

            pgo.generateProxyHelo = boolVal;
            continue;
        }


        if (name=="CUSTOMHANDLER") // CustomHandler
        {
            pgo.customHandler = value;
            continue;
        }
        if (name=="GENERATECUSTOMHANDLER")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("GenerateCustomHandler");
            }

            pgo.generateCustomHandler = boolVal;
            continue;
        }


        return setUnknownMsg(orgName);

    #if 0
    bool generateJumpEllipsis       = false; //!< генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
    bool generateJumpAll            = false; //!< генерим JUMP вместо вызова оригинальной функции для всех прокси

    bool generateHelo               = false; //!< генерировать код "Proxy called"
    bool generateCustomHandler      = false; //!< генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif
    #endif

    }

    return true;

}

