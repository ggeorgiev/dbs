//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class SysArgument;
typedef shared_ptr<SysArgument> SysArgumentSPtr;
typedef Set<SysArgument> SysArgumentSet;
typedef shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

class SysArgument : public Element<SysArgument, string>
{
public:
    SysArgument(const string& value)
        : Element(value)
    {
    }

    const string& value() const
    {
        return std::get<0>(mArgs);
    }
};
}
