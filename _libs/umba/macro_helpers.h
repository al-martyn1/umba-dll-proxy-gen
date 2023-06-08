/*! \file
    \brief Макросики-матросики. У макросов нет вопросов. Хелперы
*/

#pragma once

#include "env.h"
#include "macros.h"
#include "string_plus.h"

#include <map>
#include <string>
#include <unordered_map>


//----------------------------------------------------------------------------
// umba::macros::
namespace umba{
namespace macros{



//----------------------------------------------------------------------------
template<typename StringType>
bool getMacroTextFromMap(const StringStringMap<StringType> &m, const StringType &name, StringType &text)
{
    StringStringMap<StringType>::const_iterator it = m.find(name);
    if (it==m.end())
        return false;

    text = it->second;

    return true;
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template<typename StringType>
struct MacroTextFromMap : public IMacroTextGetter<StringType>
{
    StringStringMap<StringType> m;

    MacroTextFromMap(const StringStringMap<StringType> &_m) : m(_m) {}

    virtual bool operator()(const StringType &name, StringType &text) const override
    {
        return getMacroTextFromMap(m, name, text);
    }

    const char* getName() const { return "MacroTextFromMap"; }

}; // struct MacroTextFromMap

//------------------------------
template<typename StringType>
struct MacroTextFromMapRef : public IMacroTextGetter<StringType>
{
    const StringStringMap<StringType> &m;

    MacroTextFromMapRef(const StringStringMap<StringType> &_m) : m(_m) {}

    virtual bool operator()(const StringType &name, StringType &text) const override
    {
        return getMacroTextFromMap(m, name, text);
    }

    const char* getName() const { return "MacroTextFromMapRef"; }

}; // struct MacroTextFromMapRef

//------------------------------
template<typename StringType>
struct MacroTextFromEnv : public IMacroTextGetter<StringType>
{
    MacroTextFromEnv() {}

    virtual bool operator()(const StringType &name, StringType &text) const override
    {
        return umba::env::getVar(name, text);
    }

    const char* getName() const { return "MacroTextFromEnv"; }

}; // struct MacroTextFromEnv

//------------------------------
template<typename StringType>
struct MacroTextFromMapOrEnv : public IMacroTextGetter<StringType>
{
    StringStringMap<StringType>     m;
    bool                            envAllowed = true; // To allow control in runtime

    MacroTextFromMapOrEnv(const StringStringMap<StringType> &_m, bool _envAllowed = true) : m(_m), envAllowed(_envAllowed) {}

    virtual bool operator()(const StringType &name, StringType &text) const override
    {
        if (getMacroTextFromMap(m, name, text))
            return true;

        if (!envAllowed)
            return false;

        return umba::env::getVar(name, text);
    }

    const char* getName() const { return "MacroTextFromMapOrEnv"; }

}; // struct MacroTextFromMapOrEnv

//------------------------------
template<typename StringType>
struct MacroTextFromMapOrEnvRef : public IMacroTextGetter<StringType>
{
    const StringStringMap<StringType> &m;
    bool                              envAllowed = true; // To allow control in runtime

    MacroTextFromMapOrEnvRef(const StringStringMap<StringType> &_m, bool _envAllowed = true) : m(_m), envAllowed(_envAllowed) {}

    virtual bool operator()(const StringType &name, StringType &text) const override
    {
        if (getMacroTextFromMap(m, name, text))
            return true;

        if (!envAllowed)
            return false;

        return umba::env::getVar(name, text);
    }

    const char* getName() const { return "MacroTextFromMapOrEnvRef"; }

}; // struct MacroTextFromMapOrEnvRef

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template<typename StreamType, typename StringType>
StreamType& printMacros( StreamType &oss, const StringType &prefix, const StringStringMap<StringType> &macros )
{
    StringStringMap<StringType>::const_iterator it = macros.begin();

    std::size_t maxName = 0;

    // Scan for max name len

    for(; it!=macros.end(); ++it)
    {
        if (it->first.size()>maxName)
            maxName = it->first.size();
    }


    it = macros.begin();

    for(; it!=macros.end(); ++it)
    {
        auto fillStr = StringType( maxName - it->first.size() + 1, ' ');
        oss << prefix << it->first << fillStr << " : " << it->second << "\n";
    }

    return oss;
}


//----------------------------------------------------------------------------




//----------------------------------------------------------------------------

} // namespace macros
} // namespace umba

// umba::macros::



