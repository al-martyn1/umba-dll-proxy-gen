/*! \file
    \brief Helpers for flags
 */

#pragma once

#include "marty_enum.h"
#include "marty_flag_ops.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <cwctype>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
#define MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )
//        MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )

#define MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( val, valStr )\
        MARTY_CPP_ENUM_SERIALIZE_ITEM( val, valStr )
//        MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( val, valStr )

#define MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_SERIALIZE_END( enumTypeName, mapType, doLower )
//        MARTY_CPP_ENUM_CLASS_SERIALIZE_END( enumTypeName, mapType, doLower )

//------------------------------
#define MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )
//        MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )

#define MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( val, valStr )\
        MARTY_CPP_ENUM_DESERIALIZE_ITEM( val, valStr )
//        MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( val, valStr )

#define MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_DESERIALIZE_END( enumTypeName, mapType, doLower )
//        MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( enumTypeName, mapType, doLower )

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(enumTypeName, setType)                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_flags_##enumTypeName( enumTypeName enumVal, const char *seps = "|+,")                 \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_flags_##enumTypeName( enumTypeName enumVal, const std::string &seps)                  \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_flags( enumTypeName enumVal, const char *seps = "|+,")                                \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_flags( enumTypeName enumVal, const std::string &seps)                                 \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize( enumTypeName enumVal, const char *seps = "|+,")                                      \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize( enumTypeName enumVal, const std::string &seps)                                       \
{                                                                                                                \
    return marty_cpp::serializeEnumFlagsImpl(enumVal, enum_serialize_##enumTypeName, seps);                      \
}


//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
#define MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(enumTypeName, setType)          \
inline                                                                       \
enumTypeName enum_deserialize_flags_##enumTypeName( const std::string &str, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                            \
    auto deser = [](const std::string &str)                                  \
    { return enum_deserialize_##enumTypeName(str); };                        \
    enumTypeName e;                                                          \
    marty_cpp::deserializeEnumFlagsImpl(e, str, deser, seps);                \
    return e;                                                                \
}                                                                            \
                                                                             \
inline                                                                       \
enumTypeName enum_deserialize_flags_##enumTypeName( const std::string &str, const std::string &seps, char quotChar = '\'' )  \
{                                                                            \
    auto deser = [](const std::string &str)                                  \
    { return enum_deserialize_##enumTypeName(str); };                        \
    enumTypeName e;                                                          \
    marty_cpp::deserializeEnumFlagsImpl(e, str, deser, seps);                \
    return e;                                                                \
}                                                                            \
                                                                             \
inline                                                                       \
void enum_deserialize_flags( enumTypeName &e, const std::string &str, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                            \
    e = enum_deserialize_flags_##enumTypeName(str, seps, quotChar);          \
}                                                                            \
                                                                             \
inline                                                                       \
void enum_deserialize_flags( enumTypeName &e, const std::string &str, const std::string &seps, char quotChar = '\'' )  \
{                                                                            \
    e = enum_deserialize_flags_##enumTypeName(str, seps, quotChar);          \
}




//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace marty_cpp
{




//-----------------------------------------------------------------------------
template< typename EnumType
        , typename EnumTypeSerializer
        > inline
std::string serializeEnumFlagsImpl( EnumType enumVal
                                  , EnumTypeSerializer serializer
                                  , const char *seps // используем только первый элемент
                                  )
{
    const char sepChar = (seps && seps[0]) ? '|' : seps[0];

    std::string res;

    typedef typename std::underlying_type<EnumType>::type  EnumUnderlyingType;

    using EnumUnderlyingUnsignedType = typename std::make_unsigned<EnumUnderlyingType>::type;

    // // Calc max bit
    // EnumUnderlyingUnsignedType curBit = ~(((EnumUnderlyingUnsignedType)-1)>>1);
    // for(; curBit; curBit>>=1)
    EnumUnderlyingUnsignedType curBit = (EnumUnderlyingUnsignedType)1;
    for(; curBit; curBit<<=1)
    {
        auto testVal = enumVal&((EnumType)curBit);
        if (testVal==0)
            continue;

        auto enumValStr = serializer(testVal);
        if (enumValStr.empty())
        {
            throw std::runtime_error("serializeEnumFlagsImpl: try to serialize unknown flag");
        }

        if (!res.empty())
            res.append(1,sepChar);

        res.append(enumValStr);

    }

    if (res.empty())
        res = "0";

    return res;
}

//-----------------------------------------------------------------------------
template< typename EnumType
        , typename EnumTypeSerializer
        > inline
std::string serializeEnumFlagsImpl( EnumType enumVal
                                  , EnumTypeSerializer serializer
                                  , const std::string &seps // используем только первый элемент
                                  )
{
    return serializeEnumFlagsImpl(enumVal, serializer, seps.c_str());
}

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
template< typename EnumType
        , typename EnumTypeDeserializer
        > inline
void deserializeEnumFlagsImpl( EnumType &enumVal // deserialize to
                             , std::string str
                             , EnumTypeDeserializer deserializer
                             , const char *seps
                             )
{
    char sep = ',';
    if (seps && *seps)
    {
        sep = *seps++;
        while(*seps)
        {
            // auto sepStr     = std::string(1,*seps++);
            // auto replaceTo  = std::string(1,sep);
            str = simple_string_replace<std::string>(str, std::string(1,*seps++), std::string(1,sep));
        }
    }

    auto isSpaceChar = [](char ch)
                         {
                             typedef char CharType;
                             return ch==(CharType)' ' || ch==(CharType)'\t' || ch==(CharType)'\r' || ch==(CharType)'\n';
                         };

    auto items = simple_seq_filter(simple_string_split(str, std::string(1,sep)), [&](auto s) { return !simple_trim(s, isSpaceChar).empty(); } );

    enumVal = (EnumType)0;

    for(const auto &i : items)
    {
        if (i=="0")
            continue;
        enumVal |= deserializer(i);
    }

}

//-----------------------------------------------------------------------------
template< typename EnumType
        , typename EnumTypeDeserializer
        > inline
void deserializeEnumFlagsImpl( EnumType &enumVal // deserialize to
                             , const std::string &str
                             , EnumTypeDeserializer deserializer
                             , const std::string &seps
                             )
{
    return deserializeEnumFlagsImpl(enumVal, str, deserializer, seps.c_str());
}




//-----------------------------------------------------------------------------
} // namespace marty_cpp





//-----------------------------------------------------------------------------

