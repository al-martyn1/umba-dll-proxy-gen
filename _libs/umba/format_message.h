#pragma once
/*! 
    \file
    \brief Форматирование сообщений при помощи макросов $(Macro)
 */

#include "macro_helpers.h"
#include "stl.h"
#include "undef_FormatMessage.h"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <sstream>
#include <stdexcept>




namespace umba {




enum class EFormatAlign : unsigned
{
    left,
    center,
    right
};




template<typename StringType>
class FormatMessage
{

    typedef typename StringType::value_type    CharType;
    //typedef typename StringType::value_type    char_type;

public:

    typedef umba::macros::StringStringMap<StringType>  macros_map_type;

protected:

    umba::macros::StringStringMap<StringType>  formattedMacros    ;
    StringType                                 messageText        ;
    bool                                       fShowbase   = true ;
    bool                                       fShowsign   = false;
    unsigned                                   uBase       = 10   ;

    //TODO: !!! Надо подумать на тему замены десятичного разделителя и разделителя разрядов


protected: // utils

    StringType getComplementString( const StringType &str, std::size_t sz, CharType fillChar=(CharType)' ')
    {
        if (str.size()>=sz)
            return StringType();
    
        return StringType( sz-str.size(), fillChar );
    }

    template<typename UnsignedType>
    static StringType formatUnsigned(UnsignedType u, UnsignedType base, std::size_t width)
    {
        StringType resStr;

        for(std::size_t i=0; i<width || u; ++i)
        {
            UnsignedType d = u % base;
            CharType ch = (CharType)(d<=10 ? d+'0' : d+'A'-10);
            resStr.append(1,ch);
            u /= base;
        }

        std::reverse(resStr.begin(), resStr.end());

        return resStr;
    }

    template< typename UnsignedType >
    static 
    typename std::enable_if<std::is_unsigned<UnsignedType>::value, typename StringType >::type
    formatIntDecimal(UnsignedType u, bool showSign = false)
    {
        StringType resStr;

        while(u)
        {
            UnsignedType d = u % 10;
            CharType ch = (CharType)(d+'0');
            resStr.append(1,ch);
            u /= 10;
        }

        if (showSign)
        {
            resStr.append(1,(CharType)'+');
        }

        std::reverse(resStr.begin(), resStr.end());

        return resStr;
    
    }

    template< typename IntType >
    static
    typename std::enable_if<std::is_signed<IntType>::value, typename StringType>::type
    formatIntDecimal(IntType intVal, bool showSign = false)
    {
        typedef std::make_unsigned<IntType>::type UnsignedType;

        StringType resStr;

        bool neg = false;

        UnsignedType u = (UnsignedType)intVal;

        if (intVal<0)
        {
            u = 0-u;
            neg = true;
        }

        while(u)
        {
            UnsignedType d = u % 10;
            CharType ch = (CharType)(d+'0');
            resStr.append(1,ch);
            u /= 10;
        }

        if (neg || showSign)
        {
            resStr.append(1,(CharType)(neg ? '-': '+'));
        }

        std::reverse(resStr.begin(), resStr.end());

        return resStr;
    }


    static StringType getUnsignedPrefix(unsigned base)
    {
        StringType resStr;

        switch(base)
        {
            case 2 :
            case 8 :
            case 16:
                 resStr.append(1,(CharType)'0'); break;
        }

        switch(base)
        {
            case 2 :
                 resStr.append(1,(CharType)'b'); break;
            case 16:
                 resStr.append(1,(CharType)'x'); break;
        }

        return resStr;
    }

    static std::size_t getUnsignedPrefixLen(unsigned base)
    {
        switch(base)
        {
            case 2 : return 2;
            case 8 : return 1;
            case 16: return 2;
            default: return 0;
        }
    }

    static unsigned getCorrectBase(unsigned base)
    {
        if (base<2)
            return 2;

        return base;

        // Or?
        // switch(base)
        // {
        //     case 2 : return 2;
        //     case 8 : return 8;
        //     case 16: return 16;
        //     default: return 10;
        // }
    }


public:

    static inline EFormatAlign  alignLeft   = EFormatAlign::left  ;
    static inline EFormatAlign  alignCenter = EFormatAlign::center;
    static inline EFormatAlign  alignRight  = EFormatAlign::right ;


    FormatMessage( const StringType &msg, const std::string &ltag=std::string() )
    : formattedMacros()
    , messageText(msg)
    {}


    StringType toString() const
    {
        return umba::macros::substMacros( messageText, umba::macros::MacroTextFromMapRef<StringType>(formattedMacros)
                                        , umba::macros::smf_KeepUnknownVars | umba::macros::smf_DisableRecursion
                                        );
    }

    operator StringType() const
    {
        return toString();
    }

    FormatMessage& showbase(bool bShow=true)
    {
        fShowbase = bShow;
        return *this;
    }

    FormatMessage& noshowbase()
    {
        return showbase(false);
    }

    FormatMessage& showsign(bool bShow=true)
    {
        fShowsign = bShow;
        return *this;
    }

    FormatMessage& noshowsign()
    {
        return showsign(false);
    }


    FormatMessage& base(unsigned b)
    {
        if (b>36)
            throw std::runtime_error("FormatMessage::base: number base is out of limit (36)");
        uBase = b;
        return *this;
    }

    FormatMessage& hex() { return base(16); }
    FormatMessage& dec() { return base(10); }
    FormatMessage& oct() { return base(8 ); }
    FormatMessage& bin() { return base(2 ); }

    //TODO: !!! Не реализовано
    //! Десятичный разделитель - между целой и дробной частью
    FormatMessage& decSep(const StringType &sep) { return *this; }

    //TODO: !!! Не реализовано
    //! Разделитель груп разрядов и размер группы
    FormatMessage& decGroup(const StringType &sep, std::size_t groupSize = 3) { return *this; }

    //TODO: !!! Не реализовано
    //! Установка нац особенностей форматирования десятичных чисел, делает decSep и decGroup
    virtual FormatMessage& locale(const std::string &ltag) { return *this; }

    FormatMessage& values(const macros_map_type &predefMacros)
    {
        typename macros_map_type::const_iterator it = predefMacros.begin();
        for(; it!=predefMacros.end(); ++it)
        {
            formattedMacros[it->first] = it->second;
        }

        return *this;
    }

    const macros_map_type& values() const
    {
        return formattedMacros;
    }

    FormatMessage& arg(const StringType &argName, const StringType &val, std::size_t fieldWidth=0, EFormatAlign align=EFormatAlign::left)
    {
        StringType completemntString = getComplementString( val, fieldWidth, (CharType)' ' /* fillChar */ );
        if (align==EFormatAlign::left)
        {
            formattedMacros[argName] = val + completemntString;
        }
        else if (align==EFormatAlign::right)
        {
            formattedMacros[argName] = completemntString + val;
        }
        else // center
        {
            std::size_t lenLeft  = completemntString.size()/2;
            std::size_t lenRight = completemntString.size()-lenLeft;
            formattedMacros[argName] = StringType(completemntString, 0, lenLeft)
                                     + val
                                     + StringType(completemntString, lenLeft)
                                     ;
        }

        return *this;

    }

    template< class T
            , typename = std::enable_if_t<std::is_integral<T>::value> >
    FormatMessage& arg(const StringType &argName, T val, std::size_t fieldWidth=0, EFormatAlign align=EFormatAlign::left)
    {
        typedef std::make_signed<T>::type   IntType;
        typedef std::make_unsigned<T>::type UnsignedType;

        StringType  valFormatted;

        if (uBase==10)
        {
            valFormatted = formatIntDecimal(val, fShowsign);

        }
        else
        {
            std::size_t prefixWidth = getUnsignedPrefixLen((UnsignedType)uBase);
            std::size_t numberWidth = prefixWidth<fieldWidth ? fieldWidth-prefixWidth : 0;

            valFormatted = fShowbase 
                         ? getUnsignedPrefix((UnsignedType)uBase) + formatUnsigned<UnsignedType>(val, getCorrectBase(uBase), numberWidth)
                         :                                          formatUnsigned<UnsignedType>(val, getCorrectBase(uBase), numberWidth)
                         ;
        }

        return arg(argName, valFormatted, fieldWidth, align);
    }

    template< class T
            , typename = std::enable_if_t<std::is_floating_point<T>::value> >
    FormatMessage& arg( const StringType &argName
                      , T val
                      , int precision=-2 //!< <0 - auto, the max number of digits after the decimal point, >0 - force number of digits
                      , std::size_t fieldWidth=0
                      , EFormatAlign align=EFormatAlign::left
                      )
    {
        std::basic_ostringstream<CharType> oss;

        if (precision<0)
        {
            oss.precision(-precision);
        }
        else
        {
            oss.precision(precision);
        }

        oss << val;

        StringType str = oss.str();

        StringType::size_type numAddZeros = 0;
        if (precision>=0)
        {
            StringType::size_type dotPos = str.rfind((CharType)'.');
            if (dotPos==str.npos)
            {
                str.append(1,(CharType)'.');
                numAddZeros = (StringType::size_type)precision;
            }
            else
            {
                StringType::size_type curNumDigits = str.size()-(dotPos+1);
                if (curNumDigits<(StringType::size_type)precision)
                {
                    numAddZeros = (StringType::size_type)precision - curNumDigits;
                }
            }
        }

        str.append(numAddZeros, (CharType)'0');

        return arg(argName, str, fieldWidth, align);
    }


}; // class FormatMessage


template<typename StreamType, typename StringType>
StreamType& operator<<(StreamType &s, const FormatMessage<StringType> &fm)
{
    s << fm.toString();
    return s;
}

    

template<typename StringType> inline
FormatMessage<StringType> formatMessage(const StringType &msg, const std::string &ltag=std::string())
{
    return FormatMessage<StringType>(msg, ltag);
}

inline
FormatMessage<std::string> formatMessage(const char *msg, const std::string &ltag=std::string())
{
    return FormatMessage<std::string>(msg, ltag);
}

inline
FormatMessage<std::wstring> formatMessage(const wchar_t *msg, const std::string &ltag=std::string())
{
    return FormatMessage<std::wstring>(msg, ltag);
}




} // namespace umba
