/*! \file
    \brief Helpers for enum
 */

#pragma once

#include "marty_cpp.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <cwctype>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>



//----------------------------------------------------------------------------
#if !defined(MARTY_CPP_NO_MARTY_CPP_EXCEPTIONS)

    #include "marty_cpp_exceptions.h"

#endif


#if defined(MARTY_CPP_NO_MARTY_CPP_EXCEPTIONS)

    #define THROW_MARTY_CPP_MARTY_ENUM_DESERIALIZE_ERROR( tymeName, str ) \
                throw std::runtime_error( tymeName " - failed to deserialize value")

#else

    #define THROW_MARTY_CPP_MARTY_ENUM_DESERIALIZE_ERROR( tymeName, str ) \
                throw marty_cpp::enum_deserialization_error( tymeName, str )

#endif

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
// Requires
//   <marty_cpp/marty_cpp.h>
//   <map> or <unordered_map>
//   <exception>
//   <stdexcept>
// to be included


//----------------------------------------------------------------------------
#define MARTY_CPP_ENUM_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )     \
inline                                                                       \
std::string enum_serialize_##enumTypeName( enumTypeName v )                  \
{                                                                            \
    static mapType< enumTypeName, std::string >  _m;                         \
    if (_m.empty())                                                          \
    {

//------------------------------
#define MARTY_CPP_ENUM_SERIALIZE_ITEM( val, valStr )                         \
        _m[val] = valStr

//------------------------------
#define MARTY_CPP_ENUM_SERIALIZE_END( enumTypeName, mapType, doLower )       \
    }                                                                        \
                                                                             \
    mapType< enumTypeName, std::string >::const_iterator it = _m.find(v);    \
    if (it==_m.end())                                                        \
        return std::string();                                                \
                                                                             \
    return it->second;                                                       \
}

//------------------------------
#define MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_SERIALIZE_BEGIN( enumTypeName, mapType, doLower )

#define MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( val, valStr )                   \
        MARTY_CPP_ENUM_SERIALIZE_ITEM( val, valStr )

#define MARTY_CPP_ENUM_CLASS_SERIALIZE_END( enumTypeName, mapType, doLower ) \
        MARTY_CPP_ENUM_SERIALIZE_END( enumTypeName, mapType, doLower )       \
inline                                                                       \
std::string enum_serialize( enumTypeName v )                                 \
{                                                                            \
    return enum_serialize_##enumTypeName(v);                                 \
}

//------------------------------
#define MARTY_CPP_ENUM_SERIALIZE_SET(enumTypeName, setType)                  \
                                                                             \
inline                                                                       \
std::string enum_serialize_set_##enumTypeName( const setType<enumTypeName> &s, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                                                                \
    return marty_cpp::serializeEnumSetImpl(s, enum_serialize_##enumTypeName, seps, quotChar);                    \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_set_##enumTypeName( const setType<enumTypeName> &s, const std::string &seps, char quotChar = '\'' )  \
{                                                                                                                \
    return marty_cpp::serializeEnumSetImpl(s, enum_serialize_##enumTypeName, seps, quotChar);                    \
}

//------------------------------
#define MARTY_CPP_ENUM_CLASS_SERIALIZE_SET(enumTypeName, setType)                                                \
        MARTY_CPP_ENUM_SERIALIZE_SET(enumTypeName, setType)                                                      \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_set( const setType<enumTypeName> &s, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                                                                \
    return marty_cpp::serializeEnumSetImpl(s, enum_serialize_##enumTypeName, seps, quotChar);                    \
}                                                                                                                \
                                                                                                                 \
inline                                                                                                           \
std::string enum_serialize_set( const setType<enumTypeName> &s, const std::string &seps, char quotChar = '\'' )  \
{                                                                                                                \
    return marty_cpp::serializeEnumSetImpl(s, enum_serialize_##enumTypeName, seps, quotChar);                    \
}

//----------------------------------------------------------------------------





//----------------------------------------------------------------------------
#define MARTY_CPP_ENUM_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )   \
inline                                                                       \
const mapType< std::string, enumTypeName >& enum_deserialize_impl_get_map_##enumTypeName( )       \
{                                                                            \
    static bool lowerCaseConvert = doLower ? true : false;                   \
    static mapType< std::string, enumTypeName >  _m;                         \
    if (_m.empty())                                                          \
    {


//------------------------------
#define MARTY_CPP_ENUM_DESERIALIZE_ITEM( val, valStr )                       \
        _m[lowerCaseConvert ? marty_cpp::toLower(std::string(valStr)) : std::string(valStr)] = val


//------------------------------
#define MARTY_CPP_ENUM_DESERIALIZE_END( enumTypeName, mapType, doLower )     \
    }                                                                        \
    return _m;                                                               \
}                                                                            \
                                                                             \
inline                                                                       \
mapType< std::string, enumTypeName >::const_iterator enum_deserialize_impl_find_##enumTypeName( const mapType< std::string, enumTypeName > &_m, const std::string &str )  \
{                                                                            \
    static bool lowerCaseConvert = doLower ? true : false;                   \
    mapType< std::string, enumTypeName >::const_iterator it = _m.find(lowerCaseConvert ? marty_cpp::toLower(str) : str); \
    return it;                                                       \
}                                                                            \
                                                                             \
inline                                                                       \
enumTypeName enum_deserialize_##enumTypeName( const std::string &str )       \
{                                                                            \
    const auto & _m = enum_deserialize_impl_get_map_##enumTypeName( );       \
                                                                             \
    auto it = enum_deserialize_impl_find_##enumTypeName(_m, str);            \
    if (it==_m.end())                                                        \
        THROW_MARTY_CPP_MARTY_ENUM_DESERIALIZE_ERROR( #enumTypeName, str );  \
                                                                             \
    return it->second;                                                       \
}                                                                            \
                                                                             \
inline                                                                       \
enumTypeName enum_deserialize_##enumTypeName( const std::string &str, enumTypeName defVal ) \
{                                                                            \
    const auto & _m = enum_deserialize_impl_get_map_##enumTypeName( );       \
                                                                             \
    auto it = enum_deserialize_impl_find_##enumTypeName(_m, str);            \
    if (it==_m.end())                                                        \
        return defVal;                                                       \
                                                                             \
    return it->second;                                                       \
}





//------------------------------
#define MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )\
        MARTY_CPP_ENUM_DESERIALIZE_BEGIN( enumTypeName, mapType, doLower )

#define MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( val, valStr ) \
        MARTY_CPP_ENUM_DESERIALIZE_ITEM( val, valStr )

#define MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( enumTypeName, mapType, doLower ) \
        MARTY_CPP_ENUM_DESERIALIZE_END( enumTypeName, mapType, doLower )     \
inline                                                                       \
void enum_deserialize( enumTypeName &deserializeTo, const std::string &str ) \
{                                                                            \
    deserializeTo = enum_deserialize_##enumTypeName(str);                    \
}                                                                            \
                                                                             \
inline                                                                       \
enumTypeName enum_deserialize( const std::string &str, enumTypeName defVal ) \
{                                                                            \
    return enum_deserialize_##enumTypeName(str, defVal);                     \
}

//------------------------------
#define MARTY_CPP_ENUM_DESERIALIZE_SET(enumTypeName, setType)                \
                                                                             \
inline                                                                       \
setType<enumTypeName> enum_deserialize_set_##enumTypeName( const std::string &str, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                            \
    auto deser = [](const std::string &str)                                  \
    { return enum_deserialize_##enumTypeName(str); };                        \
    setType<enumTypeName> s;                                                 \
    marty_cpp::deserializeEnumSetImpl(s, str, deser, seps, quotChar);        \
    return s;                                                                \
}                                                                            \
                                                                             \
inline                                                                       \
setType<enumTypeName> enum_deserialize_set_##enumTypeName( const std::string &str, const std::string &seps, char quotChar = '\'' )  \
{                                                                            \
    auto deser = [](const std::string &str)                                  \
    { return enum_deserialize_##enumTypeName(str); };                        \
    setType<enumTypeName> s;                                                 \
    marty_cpp::deserializeEnumSetImpl(s, str, deser, seps, quotChar);        \
    return s;                                                                \
}

//------------------------------
#define MARTY_CPP_ENUM_CLASS_DESERIALIZE_SET(enumTypeName, setType)          \
        MARTY_CPP_ENUM_DESERIALIZE_SET(enumTypeName, setType)                \
                                                                             \
inline                                                                       \
void enum_deserialize_set( setType<enumTypeName> &s, const std::string &str, const char *seps = ",|+", char quotChar = '\'' ) \
{                                                                            \
    s = enum_deserialize_set_##enumTypeName(str, seps, quotChar);            \
}                                                                            \
                                                                             \
inline                                                                       \
void enum_deserialize_set( setType<enumTypeName> &s, const std::string &str, const std::string &seps, char quotChar = '\'' )  \
{                                                                            \
    s = enum_deserialize_set_##enumTypeName(str, seps, quotChar);            \
}

//----------------------------------------------------------------------------






//----------------------------------------------------------------------------
namespace marty_cpp
{




//-----------------------------------------------------------------------------
#ifndef MARTY_CPP_SIMPLE_STRING_SPLIT_DECLARED
#define MARTY_CPP_SIMPLE_STRING_SPLIT_DECLARED
//-----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> simple_string_split(const StringType &str, const StringType &delim, typename StringType::size_type nSplits = -1)
{
    // std::string s = "scott>=tiger>=mushroom";
    // std::string delimiter = ">=";

    // txt = "apple#banana#cherry#orange"
    // # setting the maxsplit parameter to 1, will return a list with 2 elements!
    // x = txt.split("#", 1)

    std::vector<StringType> res;

    typename StringType::size_type curPos  = 0;
    typename StringType::size_type prevPos = 0;
    //StringType token;
    while (res.size()!=nSplits && (curPos = str.find(delim, prevPos)) != StringType::npos)
    {
        res.emplace_back(str, prevPos, curPos-prevPos);
        prevPos = curPos+delim.size();
    }

    res.emplace_back(str, prevPos);

    return res;
}

//-----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> simple_string_split(const StringType &str, const typename StringType::value_type *delim, typename StringType::size_type nSplits = -1)
{
    return simple_string_split( str, StringType(delim), nSplits);
}

//-----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> simple_string_split(const StringType &str, const typename StringType::value_type delim, typename StringType::size_type nSplits = -1)
{
    typename StringType::value_type tmpDelimStr[2] = { delim, 0 };
    return simple_string_split( str, tmpDelimStr, nSplits);
}

//-----------------------------------------------------------------------------
#endif // MARTY_CPP_SIMPLE_STRING_SPLIT_DECLARED
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#ifndef MARTY_CPP_SIMPLE_STRING_REPLACE_DECLARED
#define MARTY_CPP_SIMPLE_STRING_REPLACE_DECLARED
//-----------------------------------------------------------------------------
template<typename StringType> inline
StringType simple_string_replace(const StringType &str, const StringType &searchFor, const StringType &replaceWith, typename StringType::size_type nSplits = -1)
{
    typename StringType::size_type replaceCounter = 0;

    typename StringType::size_type curPos  = 0;
    typename StringType::size_type prevPos = 0;

    StringType res; res.reserve(str.size());

    for(; replaceCounter!=nSplits && (curPos = str.find(searchFor, prevPos)) != StringType::npos; ++replaceCounter)
    {
        res.append(str, prevPos, curPos-prevPos);
        prevPos = curPos+searchFor.size();
        res.append(replaceWith);
    }

    res.append(str, prevPos);

    return res;
}

//-----------------------------------------------------------------------------
#endif // MARTY_CPP_SIMPLE_STRING_REPLACE_DECLARED
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#ifndef MARTY_CPP_SIMPLE_SEQ_FILTER_DECLARED
#define MARTY_CPP_SIMPLE_SEQ_FILTER_DECLARED
//-----------------------------------------------------------------------------
template<typename InputIterType, typename OutputIterType, typename FilterAllowPred> inline
void simple_seq_filter(InputIterType b, InputIterType e, OutputIterType o, const FilterAllowPred &allowPred)
{
    for(; b!=e; ++b)
    {
        if (allowPred(*b))
            *o++ = *b;
    }
}

//-----------------------------------------------------------------------------
template<typename ItemType, typename FilterAllowPred> inline
std::vector<ItemType> simple_seq_filter( const std::vector<ItemType> &v, const FilterAllowPred &allowPred )
{
    std::vector<ItemType> res;
    simple_seq_filter(v.cbegin(), v.cend(), std::back_inserter(res), allowPred);
    return res;
}
//-----------------------------------------------------------------------------
#endif // MARTY_CPP_SIMPLE_SEQ_FILTER_DECLARED
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#ifndef MARTY_CPP_SIMPLE_TRIM_DECLARED
#define MARTY_CPP_SIMPLE_TRIM_DECLARED
//-----------------------------------------------------------------------------
template<typename IterType, typename ConditionType> inline
IterType trim_iter_impl( IterType b, IterType e, const ConditionType &trimCondition )
{
    for(; b!=e && trimCondition(*b); ++b) {}
    return b;
}

//-----------------------------------------------------------------------------
template<typename StringType, typename ConditionType> inline
StringType simple_trim(const StringType &str, const ConditionType &trimCondition)
{
    // if (str.empty())
    //     return str;
    auto e = trim_iter_impl(str.crbegin(), str.crend(), trimCondition).base();
    //auto b  = trim_iter_impl(str.cbegin(), er.base(), trimCondition);
    auto b  = trim_iter_impl(str.cbegin(), e, trimCondition);
    //return StringType(b,er.base());
    return StringType(b,e);
}

template<typename StringType, typename ConditionType> inline
StringType simple_ltrim(const StringType &str, const ConditionType &trimCondition)
{
    auto b = trim_iter_impl(str.cbegin(), str.cend(), trimCondition);
    return StringType(b,str.cend());
}

template<typename StringType, typename ConditionType> inline
StringType simple_rtrim(const StringType &str, const ConditionType &trimCondition)
{
    // auto er = trim_iter_impl(str.crbegin(), str.crend(), trimCondition).base();
    // return StringType(str.cbegin(),er.base());
    auto e = trim_iter_impl(str.crbegin(), str.crend(), trimCondition).base();
    return StringType(str.cbegin(),e);
}

//-----------------------------------------------------------------------------
#endif // MARTY_CPP_SIMPLE_TRIM_DECLARED
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
#ifndef MARTY_CPP_TO_UPPER_TO_LOWER_DECLARED
#define MARTY_CPP_TO_UPPER_TO_LOWER_DECLARED

inline bool    isLower( char ch )     { return (ch>='a' && ch<='z'); }
inline bool    isUpper( char ch )     { return (ch>='A' && ch<='Z'); }

inline bool    isLower( wchar_t ch )  { return (ch>=L'a' && ch<=L'z'); }
inline bool    isUpper( wchar_t ch )  { return (ch>=L'A' && ch<=L'Z'); }

inline char    toLower( char ch )     { return isUpper(ch) ? ch-'A'+'a' : ch; }
inline char    toUpper( char ch )     { return isLower(ch) ? ch-'a'+'A' : ch; }

inline wchar_t toLower( wchar_t ch )  { return isUpper(ch) ? ch-L'A'+L'a' : ch; }
inline wchar_t toUpper( wchar_t ch )  { return isLower(ch) ? ch-L'a'+L'A' : ch; }

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
toLower( const std::basic_string< CharT, Traits, Allocator > &str )
{
    std::basic_string< CharT, Traits, Allocator > resStr; resStr.reserve(str.size());
    for( auto it = str.begin(); it != str.end(); ++it )
    {
        resStr.append( 1, toLower(*it) );
    }

    return resStr;
}

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
toUpper( const std::basic_string< CharT, Traits, Allocator > &str )
{
    std::basic_string< CharT, Traits, Allocator > resStr; resStr.reserve(str.size());
    for( auto it = str.begin(); it != str.end(); ++it )
    {
        resStr.append( 1, toUpper(*it) );
    }

    return resStr;
}

#endif // MARTY_CPP_TO_UPPER_TO_LOWER_DECLARED
//-----------------------------------------------------------------------------



// Прямо сейчас мне десериализация enum'ов не нужна. Допишу потом.
#if 0
template<typename StringType, typename HandlerType> inline
void splitEnumSetStringHelper( const StringType &str, const StringType &seps, typename StringType::value_type quotChar, HandlerType handler )
{
    typedef typename StringType::value_type   CharType;
    typedef typename StringType::size_type    PosType ;

    if (seps.empty()) // разделители не заданы вообще - строку не надо/не можем разделить
    {
        handler(str);
        return;
    }

    auto isPosValid = [&](auto pos)
        {
            return pos!=str.npos && pos!=str.size();
        };

    const CharType  spaces[] = { (CharType)' ', (CharType)'\t', 0 };

    PosType startPos = str.find_first_not_of(spaces, 0);
    while(isPosValid(startPos))
    {
        bool hasStartQuot = false;

        if (str[startPos]==quotChar)
        {
        }

    }

}
#endif


//-----------------------------------------------------------------------------
template< typename EnumSetType
        , typename EnumTypeSerializer
        > inline
std::string serializeEnumSetImpl( const EnumSetType &enumValsSet
                                , EnumTypeSerializer serializer
                                , const char *seps // используем только первый элемент
                                , char quotChar = '\''
                                )
{
    const char sepChar = (seps && seps[0]) ? '+' : seps[0];

    std::string res;

    for( auto enumVal : enumValsSet )
    {
        auto enumValStr = serializer(enumVal);

        if (enumValStr.empty()) // нужно ли сериализовать значения, которые сериализовались в пустую строку? Бросить исключение? Или добавить в кавычках?
            continue;

        if (!res.empty())
            res.append(1,sepChar);

        bool quoteVal = (enumValStr.find_first_of(' ')!=enumValStr.npos) ? true : false;

        if (!quoteVal && enumValStr.size()==1 && enumValStr[0]==sepChar)
            quoteVal = true;

        if (quotChar==0)
            quoteVal = false;

        if (quoteVal)
            res.append(1,quotChar);

        res.append(enumValStr);

        if (quoteVal)
            res.append(1,quotChar);
    }

    return res;
}

//-----------------------------------------------------------------------------
template< typename EnumSetType
        , typename EnumTypeSerializer
        > inline
std::string serializeEnumSetImpl( const EnumSetType &enumValsSet
                                , EnumTypeSerializer serializer
                                , const std::string &seps // используем только первый элемент
                                , char quotChar = '\''
                                )
{
    return serializeEnumSetImpl(enumValsSet, serializer, seps.c_str(), quotChar);
}

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
template< typename EnumSetType
        , typename EnumTypeDeserializer
        > inline
void deserializeEnumSetImpl( EnumSetType &enumValsSet
                           , std::string str
                           , EnumTypeDeserializer deserializer
                           , const char *seps
                           , char quotChar = '\''
                           )
{
    char sep = ',';
    if (seps && *seps)
    {
        sep = *seps++;
        while(*seps)
        {
            str = simple_string_replace<std::string>(str, std::string(1,*seps++), std::string(1,sep));
        }
    }

    auto isSpaceChar = [](char ch)
                         {
                             typedef char CharType;
                             return ch==(CharType)' ' || ch==(CharType)'\t' || ch==(CharType)'\r' || ch==(CharType)'\n';
                         };

    auto items = simple_seq_filter(simple_string_split(str, std::string(1,sep)), [&](auto s) { return !simple_trim(s, isSpaceChar).empty(); } );

    enumValsSet.clear();

    for(const auto &i : items)
    {
        enumValsSet.insert(deserializer(i));
    }

}

//-----------------------------------------------------------------------------
template< typename EnumSetType
        , typename EnumTypeDeserializer
        > inline
void deserializeEnumSetImpl( EnumSetType &enumValsSet
                           , const std::string &str
                           , EnumTypeDeserializer deserializer
                           , const std::string &seps
                           , char quotChar = '\''
                           )
{
    deserializeEnumSetImpl(enumValsSet, str, deserializer, seps.c_str(), quotChar);
}



//-----------------------------------------------------------------------------
} // namespace marty_cpp





