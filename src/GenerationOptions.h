#pragma once

#include "utils.h"
#include "marty_cpp/marty_cpp.h"

#include "ErrInfo.h"

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

}; // struct DllProxyGenerationOptions


// За неимением гербовой пишем на простой
// Нет полноценного разбора параметров ком строки - парсим инишку с параметрами
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

            pgo.dllForwardTarget = boolVal;
            continue;
        }

        if (name=="FORWARDELLIPSIS")
        {
            if (!fromBool(value, boolVal))
            {
                return setInvalidMsg("ForwardEllipsis");
            }

            pgo.dllForwardTarget = boolVal;
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

