#pragma once

#include "umba/umba.h"
//
#include "umba/string_plus.h"

#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/sort_includes.h"



struct IsSpace
{
    bool operator()(char ch) const
    {
        return ch==' ' || ch=='\t' || ch=='\r' || ch=='\n';
    }
};

inline
std::string trim(const std::string &str)
{
    return marty_cpp::simple_trim(str, IsSpace());
}


