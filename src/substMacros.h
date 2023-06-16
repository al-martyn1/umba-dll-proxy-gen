#pragma once


#include "umba/umba.h"
//
#include "umba/string_plus.h"
#include "umba/macros.h"
#include "umba/macro_helpers.h"


//----------------------------------------------------------------------------
inline
void setMacroValueMulticase(umba::macros::StringStringMap<std::string> &macros, const std::string &name, const std::string &value)
{
    macros[name] = value;
    macros[marty_cpp::toUpper(name)] = marty_cpp::toUpper(value);
    macros[marty_cpp::toLower(name)] = marty_cpp::toLower(value);
}

//------------------------------
inline
std::string substMacros( const std::string &tplStr, const umba::macros::StringStringMap<std::string> &macros )
{
    return 
    umba::macros::substMacros( tplStr
                             , umba::macros::MacroTextFromMapRef<std::string>(macros)
                             , umba::macros::smf_KeepUnknownVars | umba::macros::smf_DisableRecursion
                             );
}

//----------------------------------------------------------------------------


