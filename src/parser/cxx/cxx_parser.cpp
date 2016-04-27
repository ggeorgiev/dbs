//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"

namespace parser
{
static std::string gInclude = "include";

std::vector<std::string> CxxParser::includes(const std::string content)
{
    std::vector<std::string> result;
    size_t pos = 0;
    while (pos < content.size())
    {
        do
        {
            while (content[pos] == ' ' || content[pos] == '\t')
                ++pos;
            if (content[pos] != '#')
                break;
            ++pos;
            while (content[pos] == ' ' || content[pos] == '\t')
                ++pos;
            if (content.compare(pos, gInclude.length(), gInclude) != 0)
                break;
            pos += gInclude.length();
            while (content[pos] == ' ' || content[pos] == '\t')
                ++pos;

            char closing;
            if (content[pos] == '"')
                closing = '"';
            else if (content[pos] == '<')
                closing = '>';
            else
                break;

            size_t start = pos++;
            while (content[pos] != closing)
                ++pos;
            ++pos;

            result.push_back(content.substr(start, pos - start));
        } while (false);

        while (content[pos] != '\n' && content[pos] != '\0')
            ++pos;
        ++pos;
    }
    return result;
}
}