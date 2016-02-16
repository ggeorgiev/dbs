//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class SysArgument;
typedef std::shared_ptr<SysArgument> SysArgumentSPtr;
typedef std::unordered_set<SysArgumentSPtr> SysArgumentSet;
typedef std::shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

class SysArgument : public Base<SysArgument, std::string>
{
public:
    SysArgument(const std::string& value)
        : Base(value)
    {
    }

    const std::string& value() const
    {
        return std::get<0>(mArgs);
    }
};
}
