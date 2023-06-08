/*! \file
    \brief Helpers for namespace
 */

#pragma once

#include "marty_cpp.h"
#include "marty_ss.h"

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




//-----------------------------------------------------------------------------
template<typename StringType, typename StreamType>
class NamespaceOutputWriteGuard
{
    StreamType                       &m_oss           ;
    std::vector<StringType>          m_nsNames        ;
    NameStyle                        m_nsNameStyle    ;
    StringType                       m_nsBeginTemplate;
    StringType                       m_nsEndTemplate  ;
    StringType                       m_nsNameFormat   ;

    StringType                       m_indentSize     ;

    template<typename IterType>
    void writeNsNames(IterType b, IterType e, const StringType &tpl) const
    {
        auto isSpaceChar = [](typename StringType::value_type ch)
                             {
                                 typedef typename StringType::value_type CharType;
                                 return ch==(CharType)' ' || ch==(CharType)'\t' || ch==(CharType)'\r' || ch==(CharType)'\n';
                             };


        for(; b!=e; ++b)
        {
            auto n = simple_trim(*b, isSpaceChar);
            if (n.empty())
                continue;

            if (m_nsNameStyle!=NameStyle::defaultStyle)
                n = formatName(n, m_nsNameStyle);

            n = simple_string_replace<StringType>(m_nsNameFormat, make_string<StringType>("$(NS)"), n);
            n = simple_string_replace<StringType>(tpl, make_string<StringType>("$(NS)"), n);
            m_oss<<n;
        }
    }

    static
    std::vector<StringType> parseNsNameList( StringType nsList )
    {
        auto isSpaceChar = [](typename StringType::value_type ch)
                             {
                                 typedef typename StringType::value_type CharType;
                                 return ch==(CharType)' ' || ch==(CharType)'\t' || ch==(CharType)'\r' || ch==(CharType)'\n';
                             };

        // normalize NS separators
        nsList = simple_string_replace<StringType>(nsList, make_string<StringType>("::"), make_string<StringType>("/"));
        nsList = simple_string_replace<StringType>(nsList, make_string<StringType>(":" ), make_string<StringType>("/"));
        //nsList = simple_string_replace<StringType>(nsList, make_string<StringType>(":" ), make_string<StringType>("/"));
        nsList = simple_string_replace<StringType>(nsList, make_string<StringType>("\\"), make_string<StringType>("/"));
        return simple_seq_filter(simple_string_split(nsList, make_string<StringType>("/")), [&](auto s) { return !simple_trim(s, isSpaceChar).empty(); } );
    }


public:

    typedef StringType string_type;

    /* typename  */
    typename string_type::size_type indentSize(typename StringType::size_type nLevelSize = 4) const
    {
        return nLevelSize*m_nsNames.size();
    }

    StringType indentStr(typename StringType::size_type nLevelSize = 4) const
    {
        return StringType(indentSize(), (typename StringType::value_type)' ');
    }

    NamespaceOutputWriteGuard( NamespaceOutputWriteGuard && ) = default;
    NamespaceOutputWriteGuard( const NamespaceOutputWriteGuard & ) = default;

    NamespaceOutputWriteGuard( StreamType                   &oss
                             , const StringType             &nsList
                             , NameStyle                    nsNameStyle      = NameStyle::defaultStyle
                             , const StringType             &nsBeginTemplate = make_string<StringType>("namespace $(NS){\n")
                             , const StringType             &nsEndTemplate   = make_string<StringType>("} // $(NS)\n")
                             , const StringType             &nsNameFormat    = StringType()
                             )
    : m_oss            (oss)
    , m_nsNames        (parseNsNameList(nsList))
    , m_nsNameStyle(nsNameStyle)
    , m_nsBeginTemplate(nsBeginTemplate)
    , m_nsEndTemplate  (nsEndTemplate)
    , m_nsNameFormat   (nsNameFormat)
    {
        if (m_nsNameFormat.empty())
            m_nsNameFormat = make_string<StringType>("$(NS)");
        writeNsNames(m_nsNames.cbegin(),m_nsNames.cend(), m_nsBeginTemplate);
    }

    NamespaceOutputWriteGuard( StreamType                   &oss
                             , const StringType             &nsList
                             , const StringType             &nsNameStyle
                             , const StringType             &nsBeginTemplate = make_string<StringType>("namespace $(NS){\n")
                             , const StringType             &nsEndTemplate   = make_string<StringType>("} // $(NS)\n")
                             , const StringType             &nsNameFormat    = StringType()
                             )
    : m_oss            (oss)
    , m_nsNames        (parseNsNameList(nsList))
    , m_nsNameStyle(fromString<StringType>(nsNameStyle))
    , m_nsBeginTemplate(nsBeginTemplate)
    , m_nsEndTemplate  (nsEndTemplate)
    , m_nsNameFormat   (nsNameFormat)
    {
        if (m_nsNameFormat.empty())
            m_nsNameFormat = make_string<StringType>("$(NS)");
        writeNsNames(m_nsNames.cbegin(),m_nsNames.cend(), m_nsBeginTemplate);
    }

    ~NamespaceOutputWriteGuard()
    {
        writeNsNames(m_nsNames.crbegin(),m_nsNames.crend(), m_nsEndTemplate);
    }

};

//------------------------------
template<typename StringType, typename StreamType> inline
NamespaceOutputWriteGuard<StringType, StreamType>
makeNamespaceOutputWriteGuard( StreamType                   &oss
                             , const StringType             &nsList
                             , NameStyle                    nsNameStyle = NameStyle::defaultStyle
                             , const StringType             &nsBeginTemplate = make_string<StringType>("namespace $(NS){\n")
                             , const StringType             &nsEndTemplate   = make_string<StringType>("} // $(NS)\n")
                             , const StringType             &nsNameFormat    = StringType()
                             )
{
    return NamespaceOutputWriteGuard<StringType, StreamType>(oss, nsList, nsNameStyle, nsBeginTemplate, nsEndTemplate, nsNameFormat);
}

//------------------------------
template<typename StringType, typename StreamType> inline
NamespaceOutputWriteGuard<StringType, StreamType>
makeNamespaceOutputWriteGuard( StreamType                   &oss
                             , const StringType             &nsList
                             , const StringType             &nsNameStyle
                             , const StringType             &nsBeginTemplate = make_string<StringType>("namespace $(NS){\n")
                             , const StringType             &nsEndTemplate   = make_string<StringType>("} // $(NS)\n")
                             , const StringType             &nsNameFormat    = StringType()
                             )
{
    return NamespaceOutputWriteGuard<StringType, StreamType>(oss, nsList, nsNameStyle, nsBeginTemplate, nsEndTemplate, nsNameFormat);
}



//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
} // namespace marty_cpp



