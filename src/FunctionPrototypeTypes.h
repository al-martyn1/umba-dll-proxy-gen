#pragma once

#include <string>
#include <vector>


struct FunctionInfo;
struct FunctionArgInfo;


struct FunctionInfo
{
    std::string                     retType;
    std::string                     ptrType;
    std::string                     name   ;
    std::vector<FunctionArgInfo>    args   ;

    bool hasEllipsisArg() const;
    bool voidReturn() const
    {
        return retType=="void" || retType=="VOID";
    }


};

struct FunctionArgInfo
{
    bool           simple = true;
    std::string    simpleType   ;
    FunctionInfo   fnPointerType;
    std::string    name         ;

    bool isEllipsisType() const
    {
        if (!simple)
            return false;

        return simpleType=="...";
    }
};

inline
bool FunctionInfo::hasEllipsisArg() const
{
    for(const auto &a: args)
    {
        if (a.isEllipsisType())
            return true;
    }

    return false;
}



