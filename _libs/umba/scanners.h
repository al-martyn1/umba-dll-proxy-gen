/*! 
    \file
    \brief Разные сканеры имен. По маскам и не только. По файловой системе и не только
 */

#pragma once


#include "filename.h"
#include "filesys.h"
#include "info_log.h"
#include "regex_helpers.h"
#include "umba.h"

#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>



// umba::scanners::
namespace umba {
namespace scanners {




//----------------------------------------------------------------------------
//! Сканирует каталоги начиная с заданного и выше, в поисках конфиг файла flagsFileName
template<typename StringType> inline
StringType scanForOptionsFile( const StringType &flagsFileName, StringType basePath)
{
    while(!basePath.empty())
    {
        StringType testName = umba::filename::appendPath(basePath,flagsFileName);
        if (umba::filesys::isFileReadable(testName))
        {
            return testName;
        }

        std::string nextBasePath = umba::filename::getPath(basePath);
        if (basePath==nextBasePath)
            nextBasePath.clear();

        basePath = nextBasePath;
    }

    return StringType();
}

//----------------------------------------------------------------------------
//! Сканирует каталоги начиная с заданного и выше, в поисках конфиг файлов flagsFileNames
template<typename StringType> inline
StringType scanForOptionsFile( const std::vector<StringType> &flagsFileNames, StringType basePath)
{
    while(!basePath.empty())
    {
        for(const auto fileName : flagsFileNames)
        {
            StringType testName = umba::filename::appendPath(basePath,fileName);
            if (umba::filesys::isFileReadable(testName))
            {
                return testName;
            }
        }

        std::string nextBasePath = umba::filename::getPath(basePath);
        if (basePath==nextBasePath)
            nextBasePath.clear();

        basePath = nextBasePath;
    }

    return StringType();
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType scanForFlagsFile( const StringType &flagsFileName, StringType basePath)
{
    return scanForOptionsFile(flagsFileName, basePath);
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType scanForFlagsFile( const std::vector<StringType> &flagsFileNames, StringType basePath)
{
    return scanForOptionsFile(flagsFileNames, basePath);
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector< std::basic_regex<typename StringType::value_type> >
fromSimpleMaskToRegexVector(const std::vector<StringType> &regexStrings, bool useAnchoring = false, bool allowRawRegexes = true)
{
    std::vector< std::basic_regex<typename StringType::value_type> > resVec; resVec.reserve(regexStrings.size());

    for(const auto &s : regexStrings)
    {
        resVec.emplace_back(std::basic_regex<StringType::value_type>(umba::regex_helpers::expandSimpleMaskToEcmaRegex(s, useAnchoring, allowRawRegexes)));
    }

    return resVec;
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector< std::basic_regex<typename StringType::value_type> >
fromSimpleMaskToRegexMap(const std::vector<StringType> &regexStrings, bool useAnchoring = false, bool allowRawRegexes = true)
{
    std::map< StringType, std::basic_regex<typename StringType::value_type> > resMap;

    for(const auto &s : regexStrings)
    {
        resMap[s] = std::basic_regex<StringType::value_type>(umba::regex_helpers::expandSimpleMaskToEcmaRegex(s, useAnchoring, allowRawRegexes));
    }

    return resMap;
}

//auto normalizedEntryName = umba::filename::normalizePathSeparators(entryName,'/');

//----------------------------------------------------------------------------

//! Сканирует каталоги в поисках файлов, заданных масками инклюд и эксклюд - appConfig.includeFilesMaskList и appConfig.excludeFilesMaskList
/*! Если инклюд маски пусты, этап пропускается. Эксклюд маски обрабатываются в любом случае
*/
template<typename AppConfigType, typename LogMsgType> inline
void scanFolders( const AppConfigType      &appConfig        // with includeFilesMaskList and excludeFilesMaskList 
                , LogMsgType               &logMsg           // logMsg or logNul
                , std::vector<std::string> &foundFiles
                , std::vector<std::string> &excludedFiles
                , std::set<std::string>    &foundExtentions
                )
{
    using namespace umba::omanip;

    std::list<std::string> scanPaths( appConfig.scanPaths.begin(), appConfig.scanPaths.end() );

    std::map<std::string,std::regex>  excludeRegexes;
    std::map<std::string,std::string> excludeOriginalMasks;

    for(auto excludeFileMask : appConfig.excludeFilesMaskList)
    {
        //auto normalizedName = StringType normalizePathSeparators( const StringType &fileName, typename StringType::value_type pathSep = getNativePathSep<typename StringType::value_type>() )
        //auto regexStr = expandSimpleMaskToEcmaRegex(excludeFileMask);
        auto regexStr = umba::regex_helpers::expandSimpleMaskToEcmaRegex( excludeFileMask, true /* useAnchoring */, true /* allowRawRegexes */ );
        excludeRegexes      [regexStr] = std::regex(regexStr);
        excludeOriginalMasks[regexStr] = excludeFileMask;
    }

    std::map<std::string,std::regex>  includeRegexes;
    std::map<std::string,std::string> includeOriginalMasks;

    for(auto includeFileMask : appConfig.includeFilesMaskList)
    {
        //auto normalizedName = StringType normalizePathSeparators( const StringType &fileName, typename StringType::value_type pathSep = getNativePathSep<typename StringType::value_type>() )
        //auto regexStr = expandSimpleMaskToEcmaRegex(includeFileMask);
        auto regexStr = umba::regex_helpers::expandSimpleMaskToEcmaRegex(includeFileMask, true /* useAnchoring */, true /* allowRawRegexes */ );

        includeRegexes      [regexStr] = std::regex(regexStr);
        includeOriginalMasks[regexStr] = includeFileMask;
    }


    bool bFound = false;

    namespace fs = std::filesystem;


    // std::smatch m;
    //  
    // if ( !regex_search(text, m, std::regex(rExpr) ) )
    //     return false;
    //  
    //  
    // std::smatch m;
    // std::regex r = std::regex(rExpr);
    // if ( !regex_search(text, m, r ) )
    //     return false;
    //  
    // std::regex("meow", std::regex::ECMAScript|std::regex::icase).

    for( std::list<std::string>::const_iterator it=scanPaths.begin(); it!=scanPaths.end(); ++it )
    {
        fs::directory_iterator scanPathDirectoryIterator;
        try
        {
            scanPathDirectoryIterator = fs::directory_iterator(*it);
        }
        catch(...)
        {
            // LOG_ERR_OPT << "invalid scan path: " << *it << "\n";
            // logMsg << 
            continue;
        }


        for (const auto & entry : scanPathDirectoryIterator)
        {
            // https://en.cppreference.com/w/cpp/filesystem/directory_entry

            if (!entry.exists())
                continue;

            //------------------------------
            std::string entryName = entry.path().string();

            if (entry.is_directory())
            {
                scanPaths.push_back(entryName);
                // std::cout << entry.path() << "\n";
                continue;
            }

            //------------------------------
            if (!entry.is_regular_file())
            {
                continue; // Какая-то шляпа попалась
            }

            //------------------------------
            entryName = umba::filename::makeCanonical(entryName);

            if (!bFound)
            {
                bFound = true;

                //if (appConfig.testVerbosity(VerbosityLevel::detailed))
                {
                    umba::info_log::printSectionHeader(logMsg, "Found Files");
                    // logMsg << "---------------------\nFound Files:" << endl << "------------" << endl;
                }
            }


            // if (appConfig.testVerbosity(VerbosityLevel::detailed))
            {
                logMsg << entryName << " - ";
            }

            auto normalizedEntryName = umba::filename::normalizePathSeparators(entryName,'/');

            //TODO: !!! Нужно что-то решать с отсутствующим расширением

            //umba::filename::

            bool addThisFile = false;
            bool excludedByIncludeMask = false;

            std::string includeRegexStr;
            std::string excludeRegexStr;

            bool matchInclude = true;
            if (!includeRegexes.empty()) // матчим только если не пусто
            {
                matchInclude = umba::regex_helpers::regexMatch(normalizedEntryName,includeRegexes,&includeRegexStr);
            }

            if (!matchInclude)
            {
                // Не подходит под инклюзивную маску
                addThisFile = false;
                excludedByIncludeMask = true;
            }
            else
            {
                addThisFile = true; // Вроде подошло, надо проверить исключения

                if (umba::regex_helpers::regexMatch(normalizedEntryName,excludeRegexes,&excludeRegexStr))
                {
                    addThisFile = false;
                    excludedByIncludeMask = false;
                }
            }

            if (addThisFile)
            {
                foundFiles.push_back(entryName);

                auto ext = umba::filename::getExt(entryName);

                foundExtentions.insert(ext);

                //if (appConfig.testVerbosity(VerbosityLevel::detailed))
                {
                    if (ext.empty())
                        ext = "<EMPTY>";
                    else
                        ext = std::string(".") + ext;

                    logMsg << good << "added" << normal;
                    logMsg << " (" << notice << ext << normal << ")";
                    if (!includeRegexStr.empty())
                    {
                        // orgMask = includeOriginalMasks[includeRegexStr]
                        logMsg << " due include mask '" << includeOriginalMasks[includeRegexStr] << "' (" << includeRegexStr << ")" << normal;
                    }

                    if (ext.empty())
                    {
                        logMsg << " - !note: empty extention: " << notice << entryName << normal << "";
                        //logMsg << "\n";
                    }

                    logMsg << "\n";

                }
            }
            else
            {
                excludedFiles.push_back(entryName);

                //if (appConfig.testVerbosity(VerbosityLevel::detailed))
                {
                    if (excludedByIncludeMask)
                    {
                        logMsg << notice << "skipped" <<  /* normal << */  " due include masks" << normal << "\n";
                    }
                    else
                    {
                        logMsg << notice << "skipped" <<  /* normal << */  " due exclude mask '" << excludeOriginalMasks[excludeRegexStr] << "' (" << excludeRegexStr << ")" << normal << "\n";
                    }
                    // std::string regexStr = excludedByIncludeMask ? includeRegexStr : excludeRegexStr;
                    // std::string orgMask  = excludedByIncludeMask ? includeOriginalMasks[includeRegexStr] : excludeOriginalMasks[excludeRegexStr];
                    // std::string wichMask = excludedByIncludeMask ? "include" : "exclude";
                    // logMsg << notice << "skipped" <<  /* normal << */  " due " << wichMask << " mask '" << orgMask << "' (" << regexStr << ")" << normal << endl;
                }
            }


    // std::map<std::string,std::regex>  includeRegexes;
    // std::map<std::string,std::string> includeOriginalMasks;

            #if 0
            std::string regexStr;
            if (!umba::regex_helpers::regexMatch(normalizedEntryName,excludeRegexes,&regexStr))
            {
                foundFiles.push_back(entryName);

                auto ext = umba::filename::getExt(entryName);

                foundExtentions.insert(ext);

                if (appConfig.testVerbosity(VerbosityLevel::detailed))
                {
                    if (ext.empty())
                        ext = "<EMPTY>";
                    else
                        ext = std::string(".") + ext;

                    logMsg << good << "Added" << normal;
                    logMsg << " (" << notice << ext << normal << ")";
                    logMsg << endl;
                }
            }
            else
            {
                excludedFiles.push_back(entryName);

                if (appConfig.testVerbosity(VerbosityLevel::detailed))
                    logMsg << notice << "skipped" <<  /* normal << */  " due '" << excludeOriginalMasks[regexStr] << "' (" << regexStr << ")" << normal << endl;
            }
            #endif

// error
// warning
// notice
// normal
// good

        }

    }
}




} // namespace scanners
} // namespace umba
