/*! \file
    \brief Тестируем cEscapeString, нормализацию CR LF, нормализацию trailing spaces, condenseSpacesToTabs, normalizeIndents
*/

#include "../../marty_cpp/c_escape.h"

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #define ENABLE_ONLY_ESCAPE_TEST


#ifndef ENABLE_ONLY_ESCAPE_TEST

    #include "../../marty_cpp/src_normalization.h"

    #ifndef ENABLE_ONLY_ESCAPE_TEST
        #if defined(WIN32) || defined(_WIN32)
            #include "umba/clipboard_win32.h"
        #endif
    #endif

#endif

#ifndef ENABLE_ONLY_ESCAPE_TEST
    #include "../../marty_utext/i_symbol_traits.h"
    #include "../../marty_utext/normalization.h"
    #include "../../marty_utext/symbol_traits_impl.h"
#endif



int main( int argc, char* argv[] )
{
    using umba::lout;
    using namespace umba::omanip;


    auto escapeTest = [](const std::string &str)
    {
        std::string escaped   = marty_cpp::cEscapeString(str);
        std::string unescaped = marty_cpp::cUnescapeString(escaped);
        //std::cout << "Input  : " << str << "\n";
        std::cout << "Escaped  : " << escaped << "\n";
        // //std::cout << "Unescaped: " << unescaped << "\n";
        // std::cout << "\n";
    };

    std::cout << "Escaped tests\n\n";
    escapeTest(std::string("\0",1));
    escapeTest(std::string("\0009",2));
    escapeTest(std::string("\0000",2));
    escapeTest(std::string("\0008",2));
    escapeTest(std::string("\0007",2));
    escapeTest("\r\n\t\v\\\a\b\f\"\'");
    std::cout << "\n";


    auto unescapeTest = [](const std::string &str)
    {
        std::string unescaped = marty_cpp::cUnescapeString(str);
        std::string escaped   = marty_cpp::cEscapeString(unescaped);
        // std::cout << "Input             : " << str << "\n";
        std::cout << "Escaped unescaped : {" << escaped << "}\n";
        // //std::cout << "Unescaped: " << unescaped << "\n";
        // std::cout << "\n";
    };

    std::cout << "Unescaped tests\n\n";
    unescapeTest(std::string("\0000" ,2)); //
    unescapeTest(std::string("\\0000",4));
    unescapeTest(std::string("\\xD!" ,4)); // \r!
    unescapeTest(std::string("\\r!"  ,3)); // \r!
    unescapeTest(std::string("\\0008",5));
    unescapeTest(std::string("\\08"  ,3));
    unescapeTest(std::string("\\0007",5));
    unescapeTest(std::string("\\0G"  ,3));
    unescapeTest(std::string("\\043G",5)); // 0x23 0b00 100 011 - #G
    unescapeTest(std::string("\\x24G",5)); // $G
    unescapeTest(std::string("\0",1));
    // escapedTest("\r\n\t\v\\\a\b\f\"\'");
    std::cout << "\n";

#ifndef ENABLE_ONLY_ESCAPE_TEST

    #if defined(WIN32) || defined(_WIN32)

        HWND hwndCurConsole = umba::win32::clipboardGetConsoleHwnd();

        std::string text;

        if (!umba::win32::clipboardTextGet(text, hwndCurConsole))
        {
            std::cout << "Faild to get clipboard text\n";
        }
        else
        {
            std::cout << "Clipboard text: " << marty_cpp::cEscapeString(text) << "\n";
        }

        text = "Hello Clipboard!";
        if (!umba::win32::clipboardTextSet(text, hwndCurConsole))
        {
            std::cout << "Faild to set clipboard text\n";
        }
        else
        {
            std::cout << "Set clipboard text - Success\n";
        }

    #endif


    marty_utext::SymbolTraitsCharImpl symbolIteration;
    marty_utext::ISymbolTraitsChar *pSymbolTraits = &symbolIteration;


    auto normalizeLfTest = [&](const std::string &s)
    {
        std::cout << "Input str : [" << marty_cpp::cEscapeString(s)   << "]\n";
        std::string res   = marty_utext::normalizeLinefeeds(s, pSymbolTraits);
        std::string mcRes = marty_cpp::normalizeCrLfToLf(s);
        if (res!=mcRes)
        {
            std::cout << "!!! marty_cpp::normalizeCrLfToLf: failed\n";
        }
        std::cout << "Normalized: [" << marty_cpp::cEscapeString(res) << "]\n";
        std::cout << "\n";
    };

    std::vector<std::string> normalizeLfTestStrings =
        { "Abrigval" // Ничего вообще нет

        , "*** Mixed CR and LFs"
        , "\nLF\r\nLF\n"
        , "LF\nLF\r\r"
        , "\rLF\nLF\n"
        , "\nLF\nLF"

        , "*** DOS CR LF"
        , "CRLF\r\nCRLF"
        , "\r\nCRLF\r\nCRLF\r\n"
        , "CRLF\r\nCRLF\r\n"
        , "\r\nCRLF\r\nCRLF"

        , "*** CR only"
        , "CR\rCR"
        , "\rCR\rCR\r"
        , "CR\rCR\r"
        , "\rCR\rCR"

        , "*** LF only"
        , "LF\nLF"
        , "\nLF\nLF\n"
        , "LF\nLF\n"
        , "\nLF\nLF"

        };


    std::cout << "\nTest normalizeLinefeeds\n\n";
    for(auto testStr : normalizeLfTestStrings)
    {
        normalizeLfTest(testStr);
    }



    auto stripTrailingSpacesTest = [&](const std::string &s)
    {
        std::cout << "Input str : [" << marty_cpp::cEscapeString(s)   << "]\n";
        std::string normUtext = marty_utext::stripTrailingSpaces(s, pSymbolTraits);
        std::string normMcpp  = marty_cpp::stripTrailingSpaces(s);
        if (normUtext!=normMcpp)
        {
            std::cout << "!!! marty_cpp::stripTrailingSpaces: failed\n";
        }

        std::vector<std::string> lines = marty_utext::splitToLinesSimple(normUtext, true /* addEmptyLineAfterLastLf */ );
        std::cout << "Normalized:\n";
        for(auto line : lines)
        {
            std::cout << "[" << marty_cpp::cEscapeString(line) << "]\n";
        }
        std::cout << "\n";
    };

    std::vector<std::string> stripTrailingSpacesTestStrings =
        { "1\n2 \n 3\n 4 \n  "
        , "1\n2 \n 3\n   4 \n  \n"
        };

    std::cout << "\nTest stripTrailingSpaces\n\n";
    for(auto testStr : stripTrailingSpacesTestStrings)
    {
        stripTrailingSpacesTest(testStr);
    }


    auto tabsTest = [&](const std::string &s)
    {
        std::cout << "Input str : [" << marty_cpp::cEscapeString(s)   << "]\n";
        std::string expandedTabsStr = marty_cpp::expandTabsToSpaces(s);
        std::cout << "Expanded  : [" << marty_cpp::cEscapeString(expandedTabsStr)   << "]\n";
        std::string condecedSpacesStr = marty_cpp::condenseSpacesToTabs(expandedTabsStr);
        std::cout << "Condenced : [" << marty_cpp::cEscapeString(condecedSpacesStr)   << "]\n";
        std::string normalizedIndentsStr = marty_cpp::normalizeIndents(s);
        std::cout << "Normalized: [" << marty_cpp::cEscapeString(normalizedIndentsStr)   << "]\n";
        std::cout << "\n";
    };

    std::vector<std::string> tabsTestStrings =
        { " if (a)\n{\n    // 4 spaces\n}"
        , " if (a)\n{\n   // 3 spaces\n}"
        , " if (a)\n{\n  // 2 spaces\n}"
        , " if (a)\n{\n // 1 space\n}"
        , " if (a)\n{\n\t// 1 tab}\n"
        , " if (a)\n{\n\t // 1 tab 1 space}\n"
        , " if (a)\n{\n\t  // 1 tab 2 spaces}\n"
        , " if (a)\n{\n\t   // 1 tab 3 spaces}\n"
        };


    std::cout << "\nTest Tabs\n\n";
    for(auto testStr : tabsTestStrings)
    {
        tabsTest(testStr);
    }

#endif

    return 0;
}



