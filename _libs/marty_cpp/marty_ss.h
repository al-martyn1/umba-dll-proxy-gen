/*! \file
    \brief Simple stream template
 */

#pragma once

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




//----------------------------------------------------------------------------
namespace marty_cpp
{




//----------------------------------------------------------------------------
#ifndef MARTY_CPP_MAKE_STRING_DEFINED
#define MARTY_CPP_MAKE_STRING_DEFINED
//-----------------------------------------------------------------------------

template<typename StringType> inline StringType make_string( )
{
    return StringType();
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из std::wstring (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( const std::wstring &str )
{
    StringType res;
    for( std::wstring::const_iterator it = str.begin(); it != str.end(); ++it)
        res.append(1, (typename StringType::value_type)*it );
    return res;
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из std::string (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( const std::string &str )
{
    StringType res;
    for( std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        res.append(1, (typename StringType::value_type)*it );
    return res;
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из const wchar_t* (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( const wchar_t *str )
{
    StringType res;
    for(; *str; str++)
        res.append(1, (typename StringType::value_type)*str );
    return res;
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из const char* (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( const char *str )
{
    StringType res;
    for(; *str; str++)
        res.append(1, (typename StringType::value_type)*str );
    return res;
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из sz wchar_t'ов (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( wchar_t ch, size_t sz = 1 )
{
    StringType res;
    res.append(sz, (typename StringType::value_type)ch );
    return res;
}

//-----------------------------------------------------------------------------
//! Создаёт строку StringType из sz char'ов (работает только для базового диапазона ASCII).
template<typename StringType> inline StringType make_string( char ch, size_t sz = 1 )
{
    StringType res;
    res.append(sz, (typename StringType::value_type)ch );
    return res;
}

//-----------------------------------------------------------------------------

#endif // MARTY_CPP_MAKE_STRING_DEFINED

//----------------------------------------------------------------------------





//----------------------------------------------------------------------------
#ifndef MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL
    #define MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL( streamTypeName, fmtStr, fmtTypeName ) \
            streamTypeName & operator<<( fmtTypeName v)       \
            {                                                 \
                char buf[128];                                \
                std::sprintf(buf, fmtStr, v);                 \
                m_str.append(make_string<string_type>(buf));  \
                return *this;                                 \
            }
#endif

#ifndef MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTERS_BASIC_INTEGRAL_TYPES_IMPL
    #define MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTERS_BASIC_INTEGRAL_TYPES_IMPL( streamTypeName )           \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%d"  , int)                  \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%ld" , long)                 \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%lld", long long)            \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%u"  , unsigned)             \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%lu" , unsigned long)        \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%llu", unsigned long long)   \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%f"  , float)                \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%f"  , double)               \
            MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTER_IMPL(streamTypeName, "%Lf" , long double)
#endif

//----------------------------------------------------------------------------
#ifndef MARTY_CPP_SIMPLE_STRING_STREAM__DEFINED
#define MARTY_CPP_SIMPLE_STRING_STREAM__DEFINED
template<typename StringType>
class SimpleStringRefStream
{
    StringType          &m_str;

public:

    typedef StringType  string_type;

    typedef typename string_type::value_type         char_type        ;
    typedef typename string_type::value_type         value_type       ;
    typedef typename string_type::traits_type        traits_type      ;
    typedef typename string_type::allocator_type     allocator_type   ;
    typedef typename string_type::size_type          size_type        ;
    typedef typename string_type::difference_type    difference_type  ;
    //typedef typename string_type::


    SimpleStringRefStream( string_type &str ) : m_str(str) {}
    SimpleStringRefStream() = delete;
    SimpleStringRefStream(const SimpleStringRefStream&) = delete;
    SimpleStringRefStream(SimpleStringRefStream&&) = delete;
    SimpleStringRefStream& operator=(const SimpleStringRefStream&) = delete;

    const string_type& str() const { return m_str; }

    SimpleStringRefStream& operator<<( const std::string &str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringRefStream& operator<<( const std::wstring &str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringRefStream& operator<<( const char_type *str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringRefStream& operator<<( char_type ch )
    {
        m_str.append(1, ch);
        return *this;
    }

    SimpleStringRefStream& operator<<( bool b )
    {
        m_str.append(make_string<string_type>(b ? "true" : "false"));
        return *this;
    }

    MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTERS_BASIC_INTEGRAL_TYPES_IMPL(SimpleStringRefStream)


}; // template<StringType> class SimpleStringRefStream

//----------------------------------------------------------------------------
template<typename StringType>
class SimpleStringStream
{
    StringType          m_str;

public:

    typedef StringType  string_type;

    typedef typename string_type::value_type         char_type        ;
    typedef typename string_type::value_type         value_type       ;
    typedef typename string_type::traits_type        traits_type      ;
    typedef typename string_type::allocator_type     allocator_type   ;
    typedef typename string_type::size_type          size_type        ;
    typedef typename string_type::difference_type    difference_type  ;
    //typedef typename string_type::


    SimpleStringStream() = default;
    SimpleStringStream(const SimpleStringStream&) = delete;
    SimpleStringStream(SimpleStringStream&&) = delete;
    SimpleStringStream& operator=(const SimpleStringStream&) = delete;

    const string_type& str() const { return m_str; }

    SimpleStringStream& operator<<( const std::string &str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringStream& operator<<( const std::wstring &str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringStream& operator<<( const char_type *str )
    {
        m_str.append(make_string<string_type>(str));
        return *this;
    }

    SimpleStringStream& operator<<( char_type ch )
    {
        m_str.append(1, ch);
        return *this;
    }

    SimpleStringStream& operator<<( bool b )
    {
        m_str.append(make_string<string_type>(b ? "true" : "false"));
        return *this;
    }

    MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTERS_BASIC_INTEGRAL_TYPES_IMPL(SimpleStringStream)


}; // template<StringType> class SimpleStringStream

#endif // MARTY_CPP_SIMPLE_STRING_STREAM__DEFINED


//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
template<typename StringType>
class SimpleNulStream
{
public:

    typedef StringType  string_type;
    typedef typename string_type::value_type         char_type        ;
    typedef typename string_type::value_type         value_type       ;


    SimpleNulStream() = default;
    SimpleNulStream(const SimpleNulStream&) = delete;
    SimpleNulStream(SimpleNulStream&&) = delete;
    SimpleNulStream& operator=(const SimpleNulStream&) = delete;

    const string_type str() const { return string_type(); }

    SimpleNulStream& operator<<( const std::string &str )
    {
        return *this;
    }

    SimpleNulStream& operator<<( const std::wstring &str )
    {
        return *this;
    }

    SimpleNulStream& operator<<( const char_type *str )
    {
        return *this;
    }

    SimpleNulStream& operator<<( char_type ch )
    {
        return *this;
    }

    SimpleNulStream& operator<<( bool b )
    {
        return *this;
    }

    template<typename ValueType>
    SimpleNulStream& operator<<(ValueType v)
    {
        return *this;
    }


    // MARTY_CPP_MARTY_CPP_SIMPLE_STRING_STREAM_INSERTERS_BASIC_INTEGRAL_TYPES_IMPL(SimpleNulStream)


}; // template<StringType> class SimpleNulStream






//-----------------------------------------------------------------------------
} // namespace marty_cpp



