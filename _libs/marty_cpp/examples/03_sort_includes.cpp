/*! \file
    \brief Тестируем sort_includes.
*/

// https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes
// https://github.com/yandex/CMICOT/blob/master/CPP_STYLE_GUIDE.md#include

#include "../../marty_cpp/sort_includes.h"

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


using namespace std;



int main( int argc, char* argv[] )
{

    auto testParseSourceLineForIncludes = [](std::string s)
    {
        std::string name;
        marty_cpp::SourceLineType slt = marty_cpp::parseSourceLineForIncludes(s, &name);
        std::cout << "Source line: [" << s << "]\n";
        std::cout << "Line type  : " << (unsigned)slt;
        if (slt==marty_cpp::SourceLineType::includeLine)
        {
            std::cout << ", name: " << name;
        }

        std::cout << "\n";
        std::cout << "\n";

    };


    std::vector<std::string> parseSourceLineForIncludesTests =
        { "  #include <queue>"
        , ""
        , "//#optsortpush"
        , "//#optsortpop"
        , "//#optsortpop"
        , "//#optsortgrp0"
        , "//#optsortblk3"
        , "//#optsortsys"
        , ""
        , "#ifdef AAA"
        , "#   include string"
        , "#endif"
        , ""
        , "#   include <string>"
        , "  #include<list>"
        , ""
        , "  #  include\"queue\""
        , "#include \"deque\""
        , ""
        , ""
        , "/*#nosort*/"
        , ""
        , "#include \"vector\""
        , ""
        , "#include <set>"
        , "#include <map>"
        , ""
        , "// Continue sorting"
        , "//#sort"
        , ""
        , "#include <iterator>"
        , ""
        , "#ifdef WIN32"
        , ""
        , "    #include <windows.h>"
        , ""
        , "    #include <commctl32.h>"
        , ""
        , "#endif /* WIN32 */"
        , ""
        , "#include <utility>"
        , ""
        , "#include <algorithm>"
        , ""
        , "#include \"marty_cpp\\sort_includes.h\""
        , "#include \"marty_cpp\\marty_cpp.h\""
        , "#include \"Qt\\QtGui.h\""
        //, "// break sort"
        , "#include \"Qt\\QtCore.h\""
        , "#include \"marty_cpp\\c_escape.h\""
        , "#include \"marty_adt\\trie.h\""
        , ""
        , "namespace www{"
        , ""
        , "inline int getInt()"
        , "{"
        , "    return 3;"
        , "}"
        , ""
        , "} // namespace www"
        , ""

    for(auto t : parseSourceLineForIncludesTests)
    {
        testParseSourceLineForIncludes(t);
    }


    std::cout << "\n\n\nSort includes test #1  \n\n";

    auto testFindSortIncludes1 = [&](const std::vector<std::string> &lines)
    {
        using namespace marty_cpp;

        std::unordered_map<std::size_t, std::string> incNames;
        std::vector<SourceLineType> vecSlt  = parseSourceLinesForIncludes(lines, &incNames);
        std::vector<unsigned char>  markers = findSortIncludeMarkers(vecSlt);

        if (lines.size()!=vecSlt.size() || lines.size()!=markers.size())
        {
            std::cout << "!!! Error: line counters mismatch\n";
            return;
        }

        for(std::size_t n=0; n!=lines.size(); ++n)
        {
            std::cout << (markers[n] ? "+  " : "-  ") << lines[n];
            std::unordered_map<std::size_t, std::string>::const_iterator nit = incNames.find(n);
            if (nit!=incNames.end())
            {
                std::cout << "   [" << nit->second << "]";
            }
            std::cout << "\n";
        }

    };

    testFindSortIncludes1(parseSourceLineForIncludesTests);

    std::cout << "\n\n\nSort includes test #2  \n\n";
    auto testFindSortIncludes2 = [&](const std::vector<std::string> &inputLines)
    {
        using namespace marty_cpp;

        std::cout << "-------\n";

        auto sortedLines = sortIncludes(inputLines);

        std::unordered_map<std::size_t, std::string> incNames;
        std::vector<SourceLineType> vecSlt  = parseSourceLinesForIncludes(sortedLines, &incNames);
        std::vector<unsigned char>  markers = findSortIncludeMarkers(vecSlt);

        if (sortedLines.size()!=vecSlt.size() || sortedLines.size()!=markers.size())
        {
            std::cout << "!!! Error: line counters mismatch\n";
            return;
        }

        for(std::size_t n=0; n!=sortedLines.size(); ++n)
        {
            std::cout << (markers[n] ? "+  " : "-  ") << sortedLines[n];
            std::unordered_map<std::size_t, std::string>::const_iterator nit = incNames.find(n);
            if (nit!=incNames.end())
            {
                std::cout << "   [" << nit->second << "]";
            }
            std::cout << "\n";
        }

    };

    testFindSortIncludes2(parseSourceLineForIncludesTests);
    testFindSortIncludes2(std::vector<std::string>{ "" });
    testFindSortIncludes2(std::vector<std::string>{});

    return 0;
}



