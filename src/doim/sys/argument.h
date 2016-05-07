//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <iosfwd>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_set>

namespace doim
{
class SysArgument;
typedef std::shared_ptr<SysArgument> SysArgumentSPtr;
typedef std::unordered_set<SysArgumentSPtr> SysArgumentSet;
typedef std::shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

class SysArgument : public Base<SysArgument, std::string>
{
public:
    static SysArgumentSPtr global(const std::string& value, SysArgumentSPtr& argument);

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
