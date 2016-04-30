//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace parser
{
class CxxParser
{
public:
    enum class EIncludeType
    {
        kProgrammerDefined,
        kStandard
    };

    struct Include
    {
        EIncludeType mType;
        std::string mPath;
    };

    std::vector<Include> includes(const std::string content);
};
}
