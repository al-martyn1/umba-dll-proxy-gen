#pragma once

#include "umba/umba.h"
//

#include "FunctionPrototypeTypes.h"
#include "GenerationOptions.h"
#include "StringAppendWithSep.h"

#include "umba/string_plus.h"
#include "umba/macros.h"
#include "umba/macro_helpers.h"


#include <string>
#include <vector>
#include <unordered_map>


struct FnDefGenerateOptions
{
    std::string prototypePrefix; // EXPORT/IMPORT/etc

    bool        overrideFnType = false;
    std::string fnTypeOverride ; // WINAPI/stdcall/cdecl/etc

}; // struct FnDefGenerateOptions


inline
void clearArgNames(FunctionInfo &fi)
{
    for(auto &arg: fi.args)
    {
        arg.name.clear();
    }
}

inline
void generateArgNames(FunctionInfo &fi, const DllProxyGenerationOptions &pgo)
{
    const std::unordered_map<std::string,std::string>& initialNames = pgo.paramInitialNames; // getInitialNames();

    std::unordered_set<std::string>  usedNames;
    for(auto &arg: fi.args)
    {
        if (!arg.name.empty())
            continue;

        std::string candi;
        if (!arg.simple)
        {
            candi = "pfn";
        }
        else
        {
            if (arg.simpleType=="..." || arg.simpleType=="void" || arg.simpleType=="VOID")
                continue;

            std::unordered_map<std::string,std::string>::const_iterator it = initialNames.find(arg.simpleType);
            if (it!=initialNames.end())
                candi = it->second;
            if (candi.empty())
                candi = "arg";
        }

        std::string finalCandi = candi;
        unsigned i = 1;
        static const char *idxes = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
        while(true)
        {
            if (usedNames.find(finalCandi)==usedNames.end())
            {
                usedNames.insert(finalCandi);
                arg.name = finalCandi;
                break;
            }
            else
            {
                ++i;
                if (i>16) // !!!
                    break;
                finalCandi = candi;
                finalCandi.append(1, idxes[i]);
            }
        }
    }
}


inline
std::string generateFunctionName( const std::string &name, std::string overrideNameTemplate
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    //umba::macros::StringStringMap<std::string> m;

    macros["FunctionName"] = name;

    if (overrideNameTemplate.empty())
    {
        overrideNameTemplate = "$(FunctionName)";
    }

    return
    umba::macros::substMacros( overrideNameTemplate
                             , umba::macros::MacroTextFromMapRef<std::string>(macros)
                             , umba::macros::smf_KeepUnknownVars | umba::macros::smf_DisableRecursion
                             );
}

inline
std::string generateFunctionName( const FunctionInfo &fi, std::string overrideNameTemplate
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    return generateFunctionName(fi.name, overrideNameTemplate, macros);
}

inline
std::string generateFunctionCall( FunctionInfo fi, std::string overrideNameTemplate, FnDefGenerateOptions fnDefGenerateOptions
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                                )
{
    StringAppendWithSep argBuf = StringAppendWithSep(", ");
    for(auto argInfo: fi.args)
    {
        argBuf.append(argInfo.name);
    }

    //StringAppendWithSep buf = StringAppendWithSep(" ");

    // std::string name = fi.name;
    // if (!overrideName.empty())
    //     name = overrideName;

    return generateFunctionName(fi.name, overrideNameTemplate, macros) + "(" + argBuf.toString() + ")";
}

inline
std::string generateFunctionDef( FunctionInfo fi, bool ptrMode, std::string overrideNameTemplate, FnDefGenerateOptions fnDefGenerateOptions
                                , umba::macros::StringStringMap<std::string> macros=umba::macros::StringStringMap<std::string>()
                               )
{
    //std::string resStr;

    StringAppendWithSep buf = StringAppendWithSep(" ");

    // std::string name = fi.name;
    // if (!overrideName.empty())
    //     name = overrideName;

    std::string fnType   = fi.ptrType;
    if (fnDefGenerateOptions.overrideFnType)
    {
        fnType = fnDefGenerateOptions.fnTypeOverride;
    }

    if (ptrMode)
    {
        StringAppendWithSep ptrBuf = StringAppendWithSep(" ");
        //resStr = fi.retType + std::string(" (") + fi.ptrType + std::string(" *") + name + std::string(")");
        //buf.append(fi.retType).append("(").concat(fi.ptrType).append('*').concat(name).concat(')');

        ptrBuf.append(fnType).append('*').concat(generateFunctionName(fi, overrideNameTemplate, macros));
        buf.append(fi.retType).append("(").concat(ptrBuf.toString()).concat(')');
    }
    else
    {
        //resStr = fi.ptrType + std::string(" ") + fi.retType + std::string(" ") + name;
        buf.append(fnDefGenerateOptions.prototypePrefix).append(fi.retType).append(fnType).append(generateFunctionName(fi, overrideNameTemplate, macros));
    }

    // parameters

    buf.concat("(");
    //std::string prevSep = buf.modifySep(",")

    StringAppendWithSep argBuf = StringAppendWithSep(", ");

    for(auto argInfo: fi.args)
    {
        std::string fullArg;
        if (argInfo.simple)
        {
            fullArg = argInfo.simpleType;
            if (!argInfo.name.empty())
            {
               fullArg.append(1, ' ');
               fullArg.append(argInfo.name);
            }
        }
        else // not simple
        {
           FunctionInfo argFi = argInfo.fnPointerType;
           clearArgNames(argFi);
           fullArg = generateFunctionDef( argFi, true, argInfo.name, fnDefGenerateOptions);
        
        }

        argBuf.append(fullArg);
    }

    buf.concat(argBuf.toString());
    buf.concat(')');

    return buf.toString();

}


