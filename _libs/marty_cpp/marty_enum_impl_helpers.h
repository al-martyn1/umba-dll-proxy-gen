#pragma once

#include "sort_includes.h"

// marty_cpp::enum_impl_helpers::
namespace marty_cpp {
namespace enum_impl_helpers {



template<typename StringType> inline
StringType getStripEnumDefComment(StringType &enumDef)
{
    typedef typename StringType::value_type char_type;

    enumDef = marty_cpp::simple_trim( enumDef
                                    , [](char ch)
                                      {
                                          if (ch==(char_type)' ' || ch==(char_type)'\t')
                                              return true;
                                          return false;
                                      }
                                    );

    StringType commentStart = make_string<StringType>("//");
    StringType defSeps      = make_string<StringType>(";\r\n");
    std::size_t startsLen   = marty_cpp::sort_includes_utils::startsWith(enumDef.begin(), enumDef.end(), commentStart.c_str());
    if (startsLen==2)
    {
        enumDef.erase(0, startsLen);
        std::size_t endPos = enumDef.find_first_of(defSeps.c_str());
        if (endPos!=enumDef.npos)
        {
            StringType comment = StringType(enumDef, 0, endPos);
            enumDef.erase(0, endPos+1);
            return comment;
        }
    }
    // else
    // {
    //     return StringType();
    // }

    return StringType();

}

} // namespace enum_impl_helpers
} // namespace marty_cpp

