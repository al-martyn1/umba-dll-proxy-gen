#pragma once

#include "GenerationOptions.h"
#include "InputData.h"
#include "StringAppendWithSep.h"


template<typename StreamType> inline
bool generateEllipsisReport(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    for(auto fi: inputData.functionInfos)
    {
       if (fi.hasEllipsisArg())
       {
           oss << "Ellipsis function: " << fi.name << "\n";
       }
    }

    return true;
}


// struct DllProxyGenerationOptions
// {
//     std::string dllForwardTarget; //!<
//  
//     bool forwardData                = false; //!< форвардим все данные
//     bool forwardEllipsis            = false; //!< форвардим все функции с элипсисом
//  
//     bool generateJumpEllipsis       = false; //!< генерим JUMP вместо вызова оригинальной функции для функций с ELLIPSIS
//     bool generateJumpAll            = false; //!< генерим JUMP вместо вызова оригинальной функции для всех прокси
//  
//     bool generateHelo               = false; //!< генерировать код "Proxy called"
//     bool generateCustomHandler      = false; //!< генерировать код пользовательских обработчиков через макросы #ifdef HANDLER HANDLER() #endif
//  
// }; // struct DllProxyGenerationOptions



// struct ModuleExportEntry
// {
//     static const unsigned DefFileFlagNoname  = 0x0001;
//     static const unsigned DefFileFlagPrivate = 0x0002;
//     static const unsigned DefFileFlagData    = 0x0004;
//  
//     std::string       entryName    ;         // экспортируемое из модуля имя
//     std::string       internalName ;         // внутреннее имя/
//     std::string       otherModule  ;
//     std::string       exportedName ; // other_module exported_name
//     unsigned          ordinal      = 0;
//     unsigned          attrs        = 0; // DefFileFlag*
//  
//     bool isForwardEntry() const
//     {
//         return !otherModule.empty() && !exportedName.empty();
//     }
//  
//     // Для не ForwardEntry
//     std::string getInternalName() const
//     {
//         return internalName.empty() ? entryName : internalName;
//     }
//  
//     bool isDataEntry() const
//     {
//         return (attrs&DefFileFlagData) ? true : false;
//     }
//  
// }; // struct ModuleExportEntry


// inline
// bool isForwardEntry(const ModuleExportEntry &me, const DllProxyGenerationOptions &pgo)
// {
//     if (me.isForwardEntry())
//         return true;
//  
//     if (me.isDataEntry())
//         return pgo.forwardData;
//  
//     return false;
// }
//  
// inline
// bool isForwardEntry(const FunctionInfo &fi, const DllProxyGenerationOptions &pgo)
// {
//     if (!fi.hasEllipsisArg())
//         return false;
//  
//     // Has ellipsis
//     if (pgo.forwardEllipsis)
//         return true;
//  
//     return false;
// }



template<typename StreamType> inline
bool generateFunctionTables(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( proxyGenerationOptions
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               return !me.isDataEntry();
                                                                           }
                                                                         );

    oss << "FARPROC orgSqliteFuncPointers[" << implNames.size() << "] = { 0 };\n\n";

    oss << "const char* orgSqliteFuncNames[" << implNames.size() << "] =\n";

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

inline
std::string makeFnIdxConstant(std::string s)
{
    return std::string("ORG_SQLITE3_FN_IDX_") + marty_cpp::toUpper(s);
};

template<typename StreamType> inline
bool generateProxyTypes(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( proxyGenerationOptions
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
        std::string
        generatedFunctionDef = generateFunctionDef(fi, true, "$(FunctionName)_fnptr_t", fnDefGenerateOptions);
        oss << "typedef " << generatedFunctionDef << ";\n";
    }

    oss << "\n";

    return true;
}

template<typename StreamType> inline
bool generateProxyIndexes(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( proxyGenerationOptions
                                                                         , [&](const ModuleExportEntry &me)
                                                                           {
                                                                               return !me.isDataEntry();
                                                                           }
                                                                         );

    unsigned idx = (unsigned)-1;
    for(auto exportName: implNames)
    {
        ++idx;
        oss << "#define " << makeFnIdxConstant(exportName) << "        " << idx << "\n";
    }

    oss << "\n";

    return true;
}

template<typename StreamType> inline
bool generateDef(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    oss << "LIBRARY \"" << proxyGenerationOptions.dllTarget << "\"\n";
    oss << "EXPORTS\n";

    for(const auto &me: inputData.moduleEntries)
    {
        oss << "" << me.entryName;
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

template<typename StreamType> inline
bool generateProxyCode(StreamType &oss, ErrInfo &errInfo, const InputData &inputData, DllProxyGenerationOptions &proxyGenerationOptions)
{
    std::vector<std::string> implNames = inputData.getImplementProxyNames( proxyGenerationOptions
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

        if (me.isDataEntry())
        {
            oss << "PROXY_DATA_EXPORT " << "PROXY_SQLITE3_" << marty_cpp::toUpper(implName) << "_DATA();\n\n";
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
        fnDefGenerateOptions.prototypePrefix = "PROXY_EXPORT";

        generateArgNames(fi);

        FunctionInfo fiClr = fi;
        clearArgNames(fiClr);

        std::string
        generatedFunctionDef = generateFunctionDef(fi, false, "$(FunctionName)", fnDefGenerateOptions);
        oss << "" << generatedFunctionDef << "\n";
        oss << "{\n";

        oss << "    WHATSAPP_TRACE((\"Proxy called: %s\\n\", \"" << generateFunctionName(fi, "$(FunctionName)") << "\"));\n";

        if (!fi.hasEllipsisArg())
        {
            oss << "    " << generateFunctionName(fi, "$(FunctionName)_fnptr_t")
                          << " orgFnPtr = getOriginalFunctionPtr<" << generateFunctionName(fi, "$(FunctionName)_fnptr_t") << ">("
                          << makeFnIdxConstant(fi.name) << ");\n";
            oss << "    " << (fi.voidReturn() ? "" : "return ") << generateFunctionCall(fi, "orgFnPtr", fnDefGenerateOptions) << ";\n";
        }
        else
        {
            oss << "    PROXY_" << marty_cpp::toUpper(implName) << "_IMPL();\n";
        }

        oss << "}\n\n";

    }

    oss << "\n";

    return true;

}


