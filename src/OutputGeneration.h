#pragma once

#include "umba/umba.h"
//
#include "umba/string_plus.h"
#include "umba/macros.h"
#include "umba/macro_helpers.h"

#include "GenerationOptions.h"
#include "InputData.h"
#include "StringAppendWithSep.h"

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
inline
std::string getFillComplementString(std::size_t strLen, std::size_t complementToSize, char fillChar=' ')
{
    if (strLen>=complementToSize) // Дополнение не нужно, и так по длине хватает
    {
        return std::string();
    }

    std::size_t nFill = complementToSize - strLen;

    return std::string(nFill, fillChar);

}

//------------------------------
inline
std::string getFillComplementString(const std::string str, std::size_t complementTo, char fillChar=' ')
{
    return getFillComplementString(str.size(), complementTo, fillChar);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void setMacroValueMulticase(umba::macros::StringStringMap<std::string> &macros, const std::string &name, const std::string &value)
{
    macros[name] = value;
    macros[marty_cpp::toUpper(name)] = marty_cpp::toUpper(value);
    macros[marty_cpp::toLower(name)] = marty_cpp::toLower(value);
}

//------------------------------
inline
std::string substMacros( const std::string &tplStr, const umba::macros::StringStringMap<std::string> &macros )
{
    return 
    umba::macros::substMacros( tplStr
                             , umba::macros::MacroTextFromMapRef<std::string>(macros)
                             , umba::macros::smf_KeepUnknownVars | umba::macros::smf_DisableRecursion
                             );
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateConfigDefs(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    unsigned cnt = 0;

    auto generateConfigDefBool = [&](std::string optName, bool val)
    {
        std::string optFullName     = pgo.configDefPrefix + marty_cpp::toUpper(optName);
        std::string optFullNameFill = getFillComplementString(optFullName, 48);

        if (val)
        {
            oss << "#define " << optFullName; //  << "  "
            //if (!pgo.configDefOnlyForActive)
            {
                oss << optFullNameFill << " 1";
            }
            oss << "\n";
        }
        else // not val
        {
            if (!pgo.configDefOnlyForActiveOption)
            {
                oss << "#define " << optFullName; //  << "  "
                oss << optFullNameFill << " 0";
                oss << "\n";
            }
        }
    };

    generateConfigDefBool("ForwardData"            , pgo.forwardData);
    generateConfigDefBool("ForwardEllipsis"        , pgo.forwardEllipsis);
    generateConfigDefBool("GenerateCustomHandler"  , pgo.generateCustomHandler);
    // generateConfigDefBool("GenerateJumpEllipsis"   , pgo.generateJumpEllipsis);
    // generateConfigDefBool("GenerateJumpAll"        , pgo.generateJumpAll);
    // // generateConfigDefBool("" , pgo.);


    oss << "\n";

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateForwardReport(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    unsigned cnt = 0;
    for(auto me: inputData.moduleEntries)
    {
       if (me.isForwardEntry())
       {
           oss << "Forward entry: " << me.forwardFullName() << "\n";
           ++cnt;
       }
    }

    if (!cnt)
    {
        oss << "<No forward entries found>\n";
    }

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateDataReport(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    unsigned cnt = 0;
    for(auto me: inputData.moduleEntries)
    {
       if (me.isDataEntry())
       {
           oss << "Data entry: " << me.getInternalName() << "\n";
           ++cnt;
       }
    }

    if (!cnt)
    {
        oss << "<No data entries found>\n";
    }

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateEllipsisReport(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    unsigned cnt = 0;
    for(auto fi: inputData.functionInfos)
    {
       if (fi.hasEllipsisArg())
       {
           oss << "Ellipsis function: " << fi.name << "\n";
           ++cnt;
       }
    }

    if (!cnt)
    {
        oss << "<No ellipsis functions found>\n";
    }

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateFunctionTables(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               return !me.isDataEntry();
                                                                           }
                                                                         );

    
    // sqlite3orgFunc
    oss << "FARPROC " << pgo.proxyImplArrayNamesPrefix << "Pointers[" << implNames.size() << "] = { 0 };\n\n";

    oss << "const char* " << pgo.proxyImplArrayNamesPrefix << "Names[" << implNames.size() << "] =\n";

    unsigned idx = (unsigned)-1;
    for(auto exportName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(exportName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + exportName + "'";
            return false;
        }

        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;

        if (!idx)
            oss << "{ ";
        else
            oss << ", ";

        oss << "\"" << me.entryName << "\"\n";

    }

    oss << "};\n\n";

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateProxyTypes(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               if (me.isDataEntry())
                                                                                   return false;
                                                                               // Пропустить forwards
                                                                               return true;
                                                                           }
                                                                         );
    unsigned idx = (unsigned)-1;
    for(auto implName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(implName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + implName + "'";
            return false;
        }

        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;

        FnDefGenerateOptions fnDefGenerateOptions;
        clearArgNames(fi);

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", internalName);

        std::string functionPtrType = substMacros(pgo.functionPtrTypeFormat, macros);

        std::string
        generatedFunctionDef = generateFunctionDef(fi, true, functionPtrType, fnDefGenerateOptions);
        oss << "typedef " << generatedFunctionDef << ";\n";
    }

    oss << "\n";

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateProxyIndexes(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               return !me.isDataEntry();
                                                                           }
                                                                         );

    unsigned idx = (unsigned)-1;
    for(auto exportName: implNames)
    {
        ++idx;

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", exportName);
        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string fillStr = getFillComplementString(fnIdxConstantName, 64);
        
        oss << "#define " << fnIdxConstantName << fillStr << " " << idx << "\n";
    }

    oss << "\n";

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateDef(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    oss << "LIBRARY \"" << pgo.dllTarget << "\"\n";
    oss << "EXPORTS\n";

    for(const auto &me: inputData.moduleEntries)
    {
        oss << "  " << me.entryName;
        if (me.isForwardEntry())
        {
            std::string fwdTargetModule = me.otherModule;
            oss << "=" << fwdTargetModule << "." << me.exportedName;
        }
        else
        {
            if (!me.internalName.empty())
            {
                oss << "=" << me.internalName;
            }
        }

        if (me.ordinal!=(unsigned)-1)
        {
            oss << " @" << me.ordinal;
        }

        StringAppendWithSep attrBuf = StringAppendWithSep(" ");
        if (me.attrs&me.DefFileFlagNoname)
        {
            attrBuf.append("NONAME");
        }
        if (me.attrs&me.DefFileFlagPrivate)
        {
            attrBuf.append("PRIVATE");
        }
        if (me.attrs&me.DefFileFlagData)
        {
            attrBuf.append("DATA");
        }

        if (!attrBuf.empty())
        {
           oss << " " << attrBuf.toString();
        }

        oss << "\n";
    }

    oss << "\n";

    return true;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateProxyCode(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               return true;
                                                                           }
                                                                         );

    unsigned idx = (unsigned)-1;
    for(auto implName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(implName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + implName + "'";
            return false;
        }

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", implName);
        setMacroValueMulticase(macros, "DataName"    , implName);


        if (me.isDataEntry())
        {
            std::string dataEntryText = substMacros(pgo.proxyDataFormat, macros);
            if (!dataEntryText.empty())
            {
                oss << "" << dataEntryText << ";\n\n";
            }
            continue;
        }

        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;


        FnDefGenerateOptions fnDefGenerateOptions;
        fnDefGenerateOptions.prototypePrefix = pgo.proxyFunctionImplementationPrefix; //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string
        generatedFunctionDef = generateFunctionDef(fi, false, "$(FunctionName)", fnDefGenerateOptions);
        oss << "" << generatedFunctionDef << "\n";
        oss << "{\n";


        if (pgo.generateProxyHelo)
        {
            std::string proxyHelo = substMacros( pgo.proxyHelo, macros );
            if (!proxyHelo.empty())
            {
                oss << "    " << proxyHelo << ";\n";
            }
        }

        std::string functionPtrType   = substMacros(pgo.functionPtrTypeFormat, macros);
        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string customHandler     = substMacros(pgo.customHandlerFormat, macros);

        if (!fi.hasEllipsisArg())
        {
            oss << "    " << functionPtrType // generateFunctionName(fi, "$(FunctionName)_fnptr_t")
                          << " orgFnPtr = " << pgo.getOriginalFunctionPtrFuncTemplateName << "<" << functionPtrType /* generateFunctionName(fi, "$(FunctionName)_fnptr_t") */  << ">("
                          << fnIdxConstantName << ");\n";
            if (pgo.generateCustomHandler && !customHandler.empty())
            {
                oss << "    #if defined(" << customHandler << ")\n";
                oss << "        " << customHandler << "();\n";
                oss << "    #endif\n";
            }
            oss << "    " << (fi.voidReturn() ? "" : "return ") << generateFunctionCall(fi, "orgFnPtr", fnDefGenerateOptions) << ";\n";
        }
        else
        {
            std::string ellipsisImpl = substMacros( pgo.ellipsisImplFormat, macros );
            if (!ellipsisImpl.empty())
            {
                oss << "    " << ellipsisImpl << ";\n";
            }
        }

        oss << "}\n\n";

    }

    oss << "\n";

    return true;

}

//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateHookCode(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               if (me.isDataEntry())
                                                                                   return false;
                                                                               return true;
                                                                           }
                                                                         );

    unsigned idx = (unsigned)-1;
    for(auto implName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(implName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + implName + "'";
            return false;
        }
        if (me.isDataEntry())
        {
            errInfo.errMsg = "can't hook data entry, name '" + implName + "'";
            return false;
        }

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", implName);
        setMacroValueMulticase(macros, "DataName"    , implName);


        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;


        FnDefGenerateOptions fnDefGenerateOptions;
        fnDefGenerateOptions.prototypePrefix = pgo.proxyFunctionImplementationPrefix; //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        

        std::string
        generatedFunctionDef = generateFunctionDef(fi, false, pgo.hookFunctionNameFormat /* "$(FunctionName)" */ , fnDefGenerateOptions);
        oss << "" << generatedFunctionDef << "\n";
        oss << "{\n";


        if (pgo.generateProxyHelo)
        {
            std::string proxyHelo = substMacros( pgo.proxyHelo, macros );
            if (!proxyHelo.empty())
            {
                oss << "    " << proxyHelo << ";\n";
            }
        }

        std::string functionPtrType   = substMacros(pgo.functionPtrTypeFormat, macros);
        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string customHandler     = substMacros(pgo.customHandlerFormat, macros);

        if (!fi.hasEllipsisArg())
        {
            oss << "    " << functionPtrType // generateFunctionName(fi, "$(FunctionName)_fnptr_t")
                          << " orgFnPtr = " << pgo.getOriginalFunctionPtrFuncTemplateName << "<" << functionPtrType /* generateFunctionName(fi, "$(FunctionName)_fnptr_t") */  << ">("
                          << fnIdxConstantName << ");\n";
            if (pgo.generateCustomHandler && !customHandler.empty())
            {
                oss << "    #if defined(" << customHandler << ")\n";
                oss << "        " << customHandler << "();\n";
                oss << "    #endif\n";
            }
            oss << "    " << (fi.voidReturn() ? "" : "return ") << generateFunctionCall(fi, "orgFnPtr", fnDefGenerateOptions) << ";\n";
        }
        else
        {
            std::string ellipsisImpl = substMacros( pgo.ellipsisImplFormat, macros );
            if (!ellipsisImpl.empty())
            {
                oss << "    " << ellipsisImpl << ";\n";
            }
        }

        oss << "}\n\n";

    }

    oss << "\n";

    return true;

}

//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateHookInitCode(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               if (me.isDataEntry())
                                                                                   return false;
                                                                               return true;
                                                                           }
                                                                         );
    oss << "DetourTransactionBegin();\n";
    oss << "DetourUpdateThread(GetCurrentThread());\n\n";


    unsigned idx = (unsigned)-1;
    for(auto implName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(implName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + implName + "'";
            return false;
        }
        if (me.isDataEntry())
        {
            errInfo.errMsg = "can't hook data entry, name '" + implName + "'";
            return false;
        }

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", implName);
        setMacroValueMulticase(macros, "DataName"    , implName);


        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;


        FnDefGenerateOptions fnDefGenerateOptions;
        fnDefGenerateOptions.prototypePrefix = pgo.proxyFunctionImplementationPrefix; //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string hookFunctionName  = substMacros( pgo.hookFunctionNameFormat, macros );


        oss << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "] = " << pgo.getOriginalFunctionPtrFuncName << "(" << fnIdxConstantName << ");\n";
        oss << "DetourAttach(&(PVOID&)" << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "], (PVOID)" << hookFunctionName << ");\n\n";

    }

    oss << "\n";
    oss << "DetourTransactionCommit();\n\n";

    return true;

}

//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateHookDeinitCode(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( pgo
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               if (me.isDataEntry())
                                                                                   return false;
                                                                               return true;
                                                                           }
                                                                         );
    oss << "DetourTransactionBegin();\n";
    oss << "DetourUpdateThread(GetCurrentThread());\n\n";


    unsigned idx = (unsigned)-1;
    for(auto implName: implNames)
    {
        ModuleExportEntry me;
        if (!inputData.getModuleExportEntryByName(implName, me))
        {
            errInfo.errMsg = "entry info not found for export entry '" + implName + "'";
            return false;
        }
        if (me.isDataEntry())
        {
            errInfo.errMsg = "can't hook data entry, name '" + implName + "'";
            return false;
        }

        umba::macros::StringStringMap<std::string> macros;
        setMacroValueMulticase(macros, "FunctionName", implName);
        setMacroValueMulticase(macros, "DataName"    , implName);


        std::string internalName = me.getInternalName();

        FunctionInfo fi;

        if (!inputData.getFunctionInfoByName(internalName, fi))
        {
            errInfo.errMsg = "function '" + internalName + "' not found for entry '" + me.entryName + "'";
            return false;
        }

        ++idx;


        FnDefGenerateOptions fnDefGenerateOptions;
        fnDefGenerateOptions.prototypePrefix = pgo.proxyFunctionImplementationPrefix; //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string hookFunctionName  = substMacros( pgo.hookFunctionNameFormat, macros );


        // oss << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "] = " << pgo.getOriginalFunctionPtrFuncTemplateName << "(" << fnIdxConstantName << ");\n";
        oss << "DetourDetach(&(PVOID&)" << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "], (PVOID)" << hookFunctionName << ");\n\n";

    }

    oss << "\n";
    oss << "DetourTransactionCommit();\n\n";

    return true;

}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------



//----------------------------------------------------------------------------


