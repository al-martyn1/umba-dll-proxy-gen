#pragma once

// #include "utils.h"
#include <string>


inline
std::string appendWithSep(std::string str, const std::string &sAppend, const std::string &sSep = " ")
{
    if (sAppend.empty()) // Do nothing on empty append
        return str;

    if (str.empty())
    {
        str.append(sAppend);
    }
    else
    {
        str.append(sSep);
        str.append(sAppend);
    }

    return str;
}

class StringAppendWithSep
{
    std::string  strSep;
    std::string  strBuf;

public:

    StringAppendWithSep(const std::string &sep="", const std::string &strOrg=std::string())
    : strSep(sep)
    , strBuf(strOrg)
    {}

    bool empty() const
    {
        return strBuf.empty();
    }

    std::string modifySep(std::string newSep)
    {
        std::swap(strSep, newSep);
        return newSep;
    }

    StringAppendWithSep& append(const std::string &s)
    {
        strBuf = appendWithSep(strBuf, s, strSep);
        return *this;
    }

    StringAppendWithSep& append(std::size_t n, char ch)
    {
        append(std::string(n, ch));
        return *this;
    }

    StringAppendWithSep& append(char ch)
    {
        append(1, ch);
        return *this;
    }

    StringAppendWithSep& concat(const std::string &s)
    {
        strBuf.append(s);
        return *this;
    }

    StringAppendWithSep& concat(std::size_t n, char ch)
    {
        strBuf.append(n, ch);
        return *this;
    }

    StringAppendWithSep& concat(char ch)
    {
        strBuf.append(1, ch);
        return *this;
    }

    std::string toString() const
    {
        return strBuf;
    }


};

