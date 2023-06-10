#pragma once

#include "utils.h"
#include "marty_cpp/marty_cpp.h"

#include "ErrInfo.h"
#include "FunctionPrototypeParsing.h"

#include <string>

struct DllProxyGenerationOptions
{
    std::string dllTarget       ; //!< this dll name, no ext
    std::string dllForwardTarget; //!< forward target, no ext

    bool forwardData                = false; //!< форвардим все данные
    bool forwardEllipsis            = false; //!< форвардим все функции с элипсисом

    bool generateJumpEllipsis       = false; //!< генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
    bool generateJumpAll            = false; //!< генерим JUMP вместо вызова оригинальной функции для всех прокси

    bool generateHelo               = false; //!< генерировать код "Proxy called"
    bool generateCustomHandler      = false; //!< генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif

    std::unordered_map<std::string,std::string> paramInitialNames;

}; // struct DllProxyGenerationOptions


// За неимением гербовой пишем на простой
// Нет полноценного разбора параметров ком строки - парсим инишку с параметрами

// Примеры автогенерации имён параметров:
// int sqlite3_autovacuum_pages(sqlite3* pDb, unsigned int (*pfn)(void*, const char*, unsigned int, unsigned int, unsigned int), void* pv, void (*pfn2)(void*))
// int sqlite3_backup_finish(sqlite3_backup* pBckp)
// sqlite3_backup* sqlite3_backup_init(sqlite3* pDb, const char* pcStr, sqlite3* pDb2, const char* pcStr2)

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

        std::string name  = marty_cpp::toUpper(trim(std::string(line, 0, pos)));
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

        if (name=="DLL")
        {
            pgo.dllTarget = value;
            continue;
        }

        if (name=="FORWARDTARGET")
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

        return setUnknownMsg(name);

    #if 0
    bool generateJumpEllipsis       = false; //!< генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
    bool generateJumpAll            = false; //!< генерим JUMP вместо вызова оригинальной функции для всех прокси

    bool generateHelo               = false; //!< генерировать код "Proxy called"
    bool generateCustomHandler      = false; //!< генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif
    #endif

    }

    return true;

}

