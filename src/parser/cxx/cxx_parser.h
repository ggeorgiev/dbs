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
    std::vector<std::string> includes(const std::string content);
};
}
