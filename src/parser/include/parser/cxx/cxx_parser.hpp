//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <regex>

namespace parser
{
class CxxParser
{
public:
    std::vector<std::string> includes(const std::string content)
    {
        static std::regex include_pattern(
            "(^|\n)[ \t]*#[ \t]*include[ \t]+([\"<].*?[\">])[ \t]*");

        std::vector<std::string> results;

        std::sregex_iterator next(content.begin(), content.end(), include_pattern);
        std::sregex_iterator end;
        while (next != end)
        {
            std::smatch match = *next;
            results.push_back(match[2]);
            next++;
        }

        return results;
    }
};
}
