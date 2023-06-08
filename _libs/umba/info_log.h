/*! 
    \file
    \brief Хелперы для лога
 */

#pragma once

#include "string_plus.h"
#include "umba.h"

#include "umba/simple_formatter.h"

#include <string>

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
// umba::info_log::
namespace umba {
namespace info_log {




//----------------------------------------------------------------------------
inline
const char* getSectionSeparator()
{
    static const char *infoLogSectionSeparator = // "--------------------------------------"; // 38
    "------------------------------------------------------------------------------"; // 78
    return infoLogSectionSeparator;
}


//----------------------------------------------------------------------------
template<typename StreamType> inline
StreamType& printSectionHeader( StreamType &s, std::string secCaption )
{
    using namespace umba::omanip;

    secCaption += ":";

    s << "\n";
    s << getSectionSeparator() << "\n";
    s << caption << secCaption << normal << "\n";
    s << std::string(secCaption.size(), '-');
    s << "\n";

    return s;
}

//----------------------------------------------------------------------------
template<typename StreamType> inline
StreamType& printSectionHeader( StreamType &s, const char* secCaption )
{
    return printSectionHeader( s, std::string(secCaption) );
}

//----------------------------------------------------------------------------



} // namespace info_log
} // namespace umba

