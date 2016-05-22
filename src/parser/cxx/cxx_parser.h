//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <str>
#include <vector>

namespace parser
{
class CxxParser
{
public:
    enum class EIncludeType
    {
        kProgrammer,
        kStandard
    };

    struct Include
    {
        Include(EIncludeType type, const string& path)
            : mType(type)
            , mPath(path)
        {
        }
        EIncludeType mType;
        string mPath;
    };

    std::vector<Include> includes(const string& content);
};
}
