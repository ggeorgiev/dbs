//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class SysArgument;
typedef shared_ptr<SysArgument> SysArgumentSPtr;
typedef unordered_set<SysArgumentSPtr> SysArgumentSet;
typedef shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

class SysArgument : public Base<SysArgument, string>
{
public:
    static SysArgumentSPtr global(const string& value, SysArgumentSPtr& argument);

    SysArgument(const string& value)
        : Base(value)
    {
    }

    const string& value() const
    {
        return std::get<0>(mArgs);
    }
};
}
