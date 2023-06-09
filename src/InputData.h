#pragma once

#include "umba/umba.h"
//

#include "ErrInfo.h"
#include "FunctionPrototypeTypes.h"
#include "utils.h"
#include "umba/string_plus.h"

#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/sort_includes.h"

#include <unordered_set>
#include <string>
#include <vector>


struct ModuleExportEntry
{
    static const unsigned DefFileFlagNoname  = 0x0001;
    static const unsigned DefFileFlagPrivate = 0x0002;
    static const unsigned DefFileFlagData    = 0x0004;

    std::string       entryName    ;
    std::string       internalName ;
    std::string       otherModule  ;
    std::string       exportedName ; // other_module exported_name
    unsigned          ordinal      = 0;
    unsigned          attrs        = 0; // DefFileFlag*

}; // struct ModuleExportEntry



struct InputData
{
    std::unordered_set<std::string>                   foundTypes   ;
    std::vector<FunctionInfo>                         functionInfos;
    std::unordered_map<std::string, std::size_t>      fnDefsByName ;

    bool getFunctionInfoByName( const std::string &name, FunctionInfo &fi) const
    {
        std::unordered_map<std::string, std::size_t>::const_iterator it = fnDefsByName.find(name);
        if (it==fnDefsByName.end())
            return false;

        std::size_t idx = it->second;
        if (idx>=functionInfos.size())
            return false;

        fi = functionInfos[idx];

        return true;
    }

    std::vector<ModuleExportEntry>   moduleEntries;

// struct ModuleExportEntry
// {
//     static const unsigned DefFileFlagNoname  = 0x0001;
//     static const unsigned DefFileFlagPrivate = 0x0002;
//     static const unsigned DefFileFlagData    = 0x0004;
//  
//     std::string       entryName    ;
//     std::string       internalName ;
//     std::string       otherModule  ;
//     std::string       exportedName ; // other_module exported_name
//     unsigned          ordinal      = 0;
//     unsigned          attrs        = 0; // DefFileFlag*
//  
// }; // struct ModuleExportEntry


}; // struct InputData








inline
void scanForTypes(const FunctionInfo &fnInfo, std::unordered_set<std::string> &foundTypes)
{
    foundTypes.insert(fnInfo.retType);

    for(const auto &arg: fnInfo.args)
    {
        if (arg.simple)
        {
            foundTypes.insert(arg.simpleType);
        }
        else
        {
            scanForTypes(arg.fnPointerType, foundTypes);
        }
    }
}

inline
bool parsePrototypes( const std::string                            &prototypesText
                    , ErrInfo                                      &errInfo
                    , std::unordered_set<std::string>              &foundTypes
                    , std::vector<FunctionInfo>                    &functionInfos
                    , std::unordered_map<std::string, std::size_t> &fnDefsByName
                    )
{
    std::string fileTextNormalizedLf  = marty_cpp::normalizeCrLfToLf(prototypesText);
    std::vector<std::string> lines    = marty_cpp::splitToLinesSimple(fileTextNormalizedLf);

    errInfo.lineNo = 0;

    for(auto line: lines)
    {
        ++errInfo.lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("/*")) || umba::string_plus::starts_with(line, std::string("//")) || umba::string_plus::starts_with(line, std::string("#")))
        {
            continue; // skip comment lines
        }

        if (line.back()==',')
        {
            errInfo.errMsg = "incomplete prototype (possible linefeed)";
            ///std::cout << "error incomplete prototype (possible linefeed) in line " << lineNo << "\n";
            return false;
        }

        FunctionInfo functionInfo;
        if (!splitFunctionPointerDef(line, functionInfo))
        {
            errInfo.errMsg = "failed to parse function ptr;";
            //std::cout << "failed to parse function ptr in line " << lineNo << "\n";
            return false;
        }


        if (fnDefsByName.find(functionInfo.name)!=fnDefsByName.end())
        {
            errInfo.errMsg = "function '" + functionInfo.name + "' prototype already defined";
            return false;
        }

        fnDefsByName[functionInfo.name] = functionInfos.size();
        functionInfos.emplace_back(functionInfo);

        scanForTypes(functionInfo, foundTypes);
    }

    return true;
}

inline
bool parsePrototypes( const std::string   &prototypesText
                    , ErrInfo             &errInfo
                    , InputData           &inputData
                    )
{
    return parsePrototypes(prototypesText, errInfo, inputData.foundTypes, inputData.functionInfos, inputData.fnDefsByName);
}


// Format: entryname[=internal_name|other_module.exported_name] [@ordinal [NONAME] ] [ [PRIVATE] | [DATA] ]








inline
bool parseModuleExportEntry( const std::string &str
                           , std::string       &entryName
                           , std::string       &internalName
                           , std::string       &otherModule
                           , std::string       &exportedName // other_module exported_name
                           , unsigned          &ordinal
                           , unsigned          &attrs // DefFileFlag*
                           )
{
    entryName.clear();
    internalName.clear();
    otherModule.clear();
    exportedName.clear(); // other_module exported_name
    ordinal = (unsigned)-1;
    attrs   = 0;


    enum State
    {
        stWaitEntryName,
        stReadEntryName,
        stWaitInternalNameSep,
        stWaitInternalName,
        stReadInternalName,
        stWaitExportedName,
        stReadExportedName,
        stWaitOrdinalStrSep,
        stWaitOrdinalStr,
        stReadOrdinalStr,
        stWaitAttribute,
        stReadAttribute,
    };

    //std::string internalNameTmp; // internalName | otherModule.exportedName

    // Format: entryname[=internal_name|other_module.exported_name]
    //                  [@ordinal [NONAME] ]
    //                  [ [PRIVATE] | [DATA] ]

    std::string ordinalString;

    auto parseOrdinalString = [&]() -> bool
    {
        if (ordinalString.empty())
            return false;

        unsigned ordinalTmp = 0;
        for(auto ch : ordinalString)
        {
            if (ch<'0' || ch>'9')
                return false;
            ordinalTmp *= 10;
            ordinalTmp += ch-'0';
        }

        ordinal = ordinalTmp;

        return true;
    };

    
    std::string curAttr;
    std::vector<std::string> attrsVec;

    State st = stWaitEntryName;


    for(auto ch : str)
    {
        switch (st)
        {
            case stWaitEntryName:
            {
                if (ch==' ')
                    break;
                if (ch=='=') // may be missing entry name
                    return false;
                if (ch=='@') // may be missing entry name
                    return false;
                if (ch=='.') // may be missing entry name
                    return false;

                entryName.append(1, ch);
                st = stReadEntryName;
            }
            break;

            case stReadEntryName:
            {
                if (ch==' ')
                {
                    st = stWaitInternalNameSep;
                    break;
                }
                if (ch=='=')
                {
                    st = stWaitInternalName;
                    break;
                }

                // if (ch=='@') // собака в имени вполне допустима
                // {
                //     // st = stWaitOrdinalStr;
                //     // break;
                //     return false; // Перед ординалом должен быть пробел
                // }

                if (ch=='.')
                    return false;

                entryName.append(1, ch);
            }
            break;

            case stWaitInternalNameSep:
            {
                if (ch==' ')
                    break;
                if (ch=='=')
                {
                    st = stWaitInternalName;
                    break;
                }
                if (ch=='@')
                {
                    st = stWaitOrdinalStr;
                    break;
                }
                if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
                {
                    curAttr.append(1, ch);
                    st = stReadAttribute;
                    break;
                }

                return false;
            }
            break;

            case stWaitInternalName:
            {
                if (ch==' ')
                    break;
                if (ch=='=')
                    return false;
                if (ch=='@')
                    return false;
                if (ch=='.')
                    return false;
                internalName.append(1, ch);
                st = stReadInternalName;
            }
            break;

            case stReadInternalName:
            {
                if (ch==' ')
                {
                    st = stWaitOrdinalStrSep;
                    break;
                }
                // if (ch=='=')
                //     return false;
                // if (ch=='@') // собака в имени вполне допустима
                // {
                //     st = stWaitOrdinalStr;
                //     break;
                // }
                if (ch=='.')
                {
                    otherModule = internalName;
                    internalName.clear();
                    st = stWaitExportedName;
                    break;
                }
                internalName.append(1, ch);
            }
            break;

            case stWaitExportedName:
            {
                if (ch==' ')
                    break;
                // if (ch=='=')
                //     return false;
                // if (ch=='@') // собака в имени вполне допустима
                //     return false;
                if (ch=='.')
                    return false;
                exportedName.append(1, ch);
                st = stReadExportedName;
            }
            break;

            case stReadExportedName:
            {
                if (ch==' ')
                {
                    st = stWaitOrdinalStrSep;
                    break;
                }
                // if (ch=='=')
                //     return false;
                // if (ch=='@') // собака в имени вполне допустима
                // {
                //     st = stWaitOrdinalStr;
                //     break;
                // }
                if (ch=='.')
                    return false;
                exportedName.append(1, ch);
            }
            break;

            case stWaitOrdinalStrSep:
            {
                if (ch==' ')
                    break;
                if (ch=='=')
                    return false;
                if (ch=='@')
                {
                    st = stReadOrdinalStr;
                    break;
                }
                if (ch=='.') // ждём ординал, а нашли разделитель ExportedName
                {
                    if (!exportedName.empty()) // уже есть, это второй разделитель
                        return false;
                    st = stWaitExportedName;
                    break;
                }
                if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
                {
                    curAttr.append(1, ch);
                    st = stReadAttribute;
                    break;
                }
                return false;
            }
            break;

            case stWaitOrdinalStr:
            {
                if (ch==' ')
                    return false; // break;
                if (ch=='=')
                    return false;
                if (ch=='@')
                    return false;
                if (ch=='.')
                    return false;
                if (ch<'0' || ch>'9')
                    return false;
                ordinalString.append(1, ch);
                st = stReadOrdinalStr;
            }
            break;

            case stReadOrdinalStr:
            {
                if (ch==' ')
                {
                    // распарсить ordinalString
                    if (!parseOrdinalString())
                        return false;
                    st = stWaitAttribute;
                    break;
                }
                if (ch=='=')
                    return false;
                if (ch=='@')
                    return false;
                if (ch=='.')
                    return false;
                if (ch<'0' || ch>'9')
                    return false;
                ordinalString.append(1, ch);
            }
            break;

            case stWaitAttribute:
            {
                if (ch==' ')
                    break;
                if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
                {
                    curAttr.append(1, ch);
                    st = stReadAttribute;
                    break;
                }
                return false;
            }
            break;

            case stReadAttribute:
            {
                if (ch==' ')
                {
                    attrsVec.emplace_back(curAttr);
                    curAttr.clear();
                    st = stWaitAttribute;
                    break;
                }
                if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
                {
                    curAttr.append(1, ch);
                    break;
                }
                return false;
            }
            break;

            default: return false;
        }

    } // for(auto ch : str)

    auto parseAttrs = [&]() -> bool
    {
        if (!curAttr.empty())
           attrsVec.emplace_back(curAttr);

        for(auto attr: attrsVec)
        {
            attr = marty_cpp::toUpper(attr);
            if (attr=="NONAME")
            {
                if (ordinal==(unsigned)-1)
                    return false; // NONAME attr valid only if ordinal taken

                attrs |= ModuleExportEntry::DefFileFlagNoname;
                continue;
            }

            if (attr=="PRIVATE")
            {
                attrs |= ModuleExportEntry::DefFileFlagPrivate;
                continue;
            }

            if (attr=="DATA")
            {
                attrs |= ModuleExportEntry::DefFileFlagData;
                continue;
            }

            return false;
                
        }

        return true;
    };

    switch (st)
    {
        case stWaitEntryName       : return false; // ничего нет
        case stReadEntryName       : return true ; // строка закончилась на чтении имени entry
        case stWaitInternalNameSep : return true ; // строка закончилась на ожидании после чтения entry
        case stWaitInternalName    : return false; // был найден сепаратор =, но продолжения нет
        case stReadInternalName    : return true ; // строка закончилась на чтении internal имени
        case stWaitExportedName    : return false; // 
        case stReadExportedName    : return true ;
        case stWaitOrdinalStrSep   : return true ;
        case stWaitOrdinalStr      : return false; // ordinal не дочитан
        case stReadOrdinalStr      : return parseOrdinalString(); // надо распарсить ordinal
        case stWaitAttribute       : return true ;
        case stReadAttribute       : return parseAttrs();
    }

    return false;

}

inline
bool parseModuleExportEntry( const std::string &str
                           , ModuleExportEntry &moduleExportEntry
                           )
{
    return parseModuleExportEntry( str
                                 , moduleExportEntry.entryName   
                                 , moduleExportEntry.internalName
                                 , moduleExportEntry.otherModule 
                                 , moduleExportEntry.exportedName
                                 , moduleExportEntry.ordinal     
                                 , moduleExportEntry.attrs       
                                 );
}



inline
bool parseOutputProxyFunctionsList( const std::string                 &functionsListText
                                  , ErrInfo                          &errInfo
                                  , std::vector<std::string>         &proxyFunctionsList
                                  , std::unordered_set<std::string>  &exportedData
                                  )
{
    std::string proxyFunctionsFileTextNormalizedLf       = marty_cpp::normalizeCrLfToLf(functionsListText);
    std::vector<std::string> proxyFunctionsFileTextLines = marty_cpp::splitToLinesSimple(proxyFunctionsFileTextNormalizedLf);

    errInfo.lineNo = 0;
    for(auto line: proxyFunctionsFileTextLines)
    {
        ++errInfo.lineNo;

        line = trim(line);

        if (line.empty())
        {
            continue; // skip empty lines
        }

        //std::size_t startsLen
        if (umba::string_plus::starts_with(line, std::string("/*")) || umba::string_plus::starts_with(line, std::string("//")) || umba::string_plus::starts_with(line, std::string("#")))
        {
            continue; // skip comment lines
        }

        //bool res
        umba::string_plus::starts_with_and_strip( line, std::string("sqlite3_") );

        std::size_t pos = line.find(' ');
        if (pos==line.npos)
        {
            proxyFunctionsList.emplace_back(line);
        }
        else
        {
            // Forwarding - https://devblogs.microsoft.com/oldnewthing/20121116-00/?p=6073
            // Exporting from a DLL Using DEF Files - https://learn.microsoft.com/en-us/cpp/build/exporting-from-a-dll-using-def-files?view=msvc-170
            // EXPORTS - https://learn.microsoft.com/en-us/cpp/build/reference/exports?view=msvc-170
            // Format: entryname[=internal_name|other_module.exported_name] [@ordinal [NONAME] ] [ [PRIVATE] | [DATA] ]


            std::string name = trim(std::string(line, 0, pos));
            std::string attr = marty_cpp::toUpper(trim(std::string(line, pos+1)));
            if (attr=="DATA")
            {
                exportedData.insert(name);
            }

            proxyFunctionsList.emplace_back(name);
        }

    }

    return true;

}



