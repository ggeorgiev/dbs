//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"
#include <stddef.h>

namespace parser
{
static std::string gInclude = "include";

std::vector<CxxParser::Include> CxxParser::includes(const std::string content)
{
    std::vector<Include> result;
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

            size_t start = ++pos;
            while (content[pos] != closing)
                ++pos;

            Include include({closing == '"' ? EIncludeType::kProgrammerDefined
                                            : EIncludeType::kStandard,
                             content.substr(start, pos - start)});

            result.push_back(include);
            ++pos;
        } while (false);

        while (content[pos] != '\n' && content[pos] != '\0')
            ++pos;
        ++pos;
    }
    return result;
}
}
