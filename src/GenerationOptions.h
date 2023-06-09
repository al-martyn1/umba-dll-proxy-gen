#pragma once

#include <string>

struct DllProxyGenerationOptions
{
    std::string dllForwardTarget; //!<

    bool forwardData                = false; //!< форвардим все функции с элипсисом
    bool forwardEllipsis            = false; //!< форвардим все функции с элипсисом

    bool generateJumpEllipsis       = false; //!< генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
    bool generateJumpAll            = false; //!< генерим JUMP вместо вызова оригинальной функции для всех прокси

    bool generateHelo               = false; //!< генерировать код "Proxy called"
    bool generateCustomHandler      = false; //!< генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif


}; // struct DllProxyGenerationOptions

