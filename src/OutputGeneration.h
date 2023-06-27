#pragma once

#include "umba/umba.h"
//
#include "umba/string_plus.h"
#include "umba/macros.h"
#include "umba/macro_helpers.h"

#include "GenerationOptions.h"
#include "InputData.h"
#include "StringAppendWithSep.h"

#include "substMacros.h"

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
template<typename StreamType> inline
bool generateConfigDefs(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    unsigned cnt = 0;

    auto generateConfigDefBool = [&](std::string optName, bool val)
    {
        std::string optFullName     = pgo.substMacros(pgo.configDefPrefix) + marty_cpp::toUpper(optName);
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
    oss << "FARPROC " << pgo.substMacros(pgo.proxyImplArrayNamesPrefix) << "Pointers[" << implNames.size() << "] = { 0 };\n\n";

    oss << "const char* " << pgo.substMacros(pgo.proxyImplArrayNamesPrefix) << "Names[" << implNames.size() << "] =\n";

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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; // !!!
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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
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
    oss << "LIBRARY \"" << pgo.substMacros(pgo.dllTarget) << "\"\n";
    oss << "EXPORTS\n";

    for(const auto &me: inputData.moduleEntries)
    {
        if (pgo.defFileIndent)
        {
            oss << std::string(pgo.defFileIndent, ' ');
        }
        oss << me.entryName;

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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
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
        fnDefGenerateOptions.prototypePrefix = pgo.substMacros(pgo.proxyFunctionImplementationPrefix); //  "PROXY_EXPORT";

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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
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
        fnDefGenerateOptions.prototypePrefix = pgo.substMacros(pgo.proxyFunctionImplementationPrefix); //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        

        std::string
        generatedFunctionDef = generateFunctionDef(fi, false, substMacros( pgo.hookFunctionNameFormat, macros ) /* "$(FunctionName)" */ , fnDefGenerateOptions);
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
                          << " orgFnPtr = " << substMacros(pgo.getOriginalFunctionPtrFuncTemplateName, macros ) << "<" << functionPtrType /* generateFunctionName(fi, "$(FunctionName)_fnptr_t") */  << ">("
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
    // oss << "DetourTransactionBegin();\n";
    // oss << "DetourUpdateThread(GetCurrentThread());\n\n";

    oss << pgo.substMacros(pgo.hookInitStartFormat) << ";\n\n";


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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
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
        fnDefGenerateOptions.prototypePrefix = pgo.substMacros(pgo.proxyFunctionImplementationPrefix); //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string hookFunctionName  = substMacros( pgo.hookFunctionNameFormat, macros );

        macros["FunctionIndex"]          = fnIdxConstantName;
        macros["HookFunctionName"]       = hookFunctionName;
        macros["FunctionPointersTable"]  = pgo.substMacros(pgo.proxyImplArrayNamesPrefix) + "Pointers";
        macros["GetFunctionPtr"]         = pgo.substMacros(pgo.getOriginalFunctionPtrFuncTemplateName);
        macros["GetFunctionFarprocPtr"]  = pgo.substMacros(pgo.getOriginalFunctionFarprocPtrFuncName);

        //oss << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "] = " << pgo.getOriginalFunctionFarprocPtrFuncName << "(" << fnIdxConstantName << ");\n";
        //oss << "DetourAttach(&(PVOID&)" << pgo.substMacros(pgo.proxyImplArrayNamesPrefix) << "Pointers[" << fnIdxConstantName << "], (PVOID)" << hookFunctionName << ");\n\n";
        oss << substMacros(pgo.hookInitFormat, macros) << ";\n\n";

    }

    oss << "\n";
    // oss << "DetourTransactionCommit();\n\n";
    oss << pgo.substMacros(pgo.hookInitEndFormat) << ";\n\n";

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
    // oss << "DetourTransactionBegin();\n";
    // oss << "DetourUpdateThread(GetCurrentThread());\n\n";
    oss << pgo.substMacros(pgo.hookDeinitStartFormat) << ";\n\n";


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

        umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
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
        fnDefGenerateOptions.prototypePrefix = pgo.substMacros(pgo.proxyFunctionImplementationPrefix); //  "PROXY_EXPORT";

        generateArgNames(fi, pgo);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string fnIdxConstantName = substMacros( pgo.functionIndexConstantNameFormat, macros );
        std::string hookFunctionName  = substMacros( pgo.hookFunctionNameFormat, macros );

        macros["FunctionIndex"]          = fnIdxConstantName;
        macros["HookFunctionName"]       = hookFunctionName;
        macros["FunctionPointersTable"]  = pgo.substMacros(pgo.proxyImplArrayNamesPrefix) + "Pointers";

        // oss << pgo.proxyImplArrayNamesPrefix << "Pointers[" << fnIdxConstantName << "] = " << pgo.getOriginalFunctionPtrFuncTemplateName << "(" << fnIdxConstantName << ");\n";
        //oss << "DetourDetach(&(PVOID&)" << pgo.substMacros(pgo.proxyImplArrayNamesPrefix) << "Pointers[" << fnIdxConstantName << "], (PVOID)" << hookFunctionName << ");\n\n";
        oss << substMacros(pgo.hookDeinitFormat, macros) << ";\n\n";

    }

    oss << "\n";
    //oss << "DetourTransactionCommit();\n\n";
    oss << pgo.substMacros(pgo.hookDeinitEndFormat) << ";\n\n";

    return true;

}

//----------------------------------------------------------------------------
template<typename StreamType> inline
bool generateGetProcPtrFuncs(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &pgo)
{
    umba::macros::StringStringMap<std::string> macros = pgo.predefinedMacros; //!!!
    macros["FunctionPointersTable"]  = pgo.substMacros(pgo.proxyImplArrayNamesPrefix) + "Pointers";
    macros["FunctionNamesTable"]     = pgo.substMacros(pgo.proxyImplArrayNamesPrefix) + "Names";
    macros["GetFunctionPtr"]         = pgo.substMacros(pgo.getOriginalFunctionPtrFuncTemplateName);
    macros["GetFunctionFarprocPtr"]  = pgo.substMacros(pgo.getOriginalFunctionFarprocPtrFuncName);

    setMacroValueMulticase(macros, "ProxyDll"     , pgo.substMacros(pgo.dllTarget)       );
    setMacroValueMulticase(macros, "HookDll"      , pgo.substMacros(pgo.dllTarget)       );
    setMacroValueMulticase(macros, "ForwardTarget", pgo.substMacros(pgo.dllForwardTarget));

    macros["GetModuleHandleApiFn"]   = pgo.useLoadLibrary ? "LoadLibraryA" : "GetModuleHandleA";

    // static HMODULE hMdod = GetModuleHandleA("e_sqlite3.dll");
    // static HMODULE hMdod = LoadLibraryA("e_sqlite3_org.dll");

    oss << substMacros( 
"// INI: ProxyImplArrayNamesPrefix, FunctionPtrTypeFormat, GetOriginalFunctionPtrFuncTemplateName, getOriginalFunctionFarprocPtrFuncName"
"inline\n"
"HMODULE getOriginal$(ModuleName)Hmodule()\n"
"{\n"
"    static HMODULE hMdod = $(GetModuleHandleApiFn)(\"$(forwardtarget).dll\");\n"
"    return hMdod;\n"
"}\n"
"\n"
"inline\n"
"FARPROC $(GetFunctionFarprocPtr)(unsigned idx)\n"
"{\n"
"    return GetProcAddress(getOriginal$(ModuleName)Hmodule(), $(FunctionNamesTable)[idx]);\n"
"}\n"
"\n"
"template<typename FnPtrType>\n"
"FnPtrType $(GetFunctionPtr)(unsigned idx)\n"
"{\n"
"    return reinterpret_cast<FnPtrType>($(FunctionPointersTable)[idx]);\n"
"}\n"
"\n"
                      , macros);

// HMODULE getOriginalKernel32Hmodule()
// {
//     static HMODULE hMdod = GetModuleHandleA("kernel32.dll");
//     return hMdod;
// }
//  
// FARPROC getOriginalKernel32FunctionFarprocPtr(unsigned idx)
// {
//     return GetProcAddress(getOriginalKernel32Hmodule(), fnTableKernel32Names[idx]);
// }
//  
// template<typename FnPtrType>
// FnPtrType getOriginalKernel32FunctionPtr(unsigned idx)
// {
//     return reinterpret_cast<FnPtrType>(fnTableKernel32Pointers[idx]);
// }

    return true;

}



//----------------------------------------------------------------------------



//----------------------------------------------------------------------------


