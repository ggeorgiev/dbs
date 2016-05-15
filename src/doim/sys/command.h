//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/sys/argument.h"
#include "doim/sys/executable.h"
#include "dp/simple_memoization.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class SysCommand;
typedef shared_ptr<SysCommand> SysCommandSPtr;

class SysCommand : public Base<SysCommand, SysExecutableSPtr, SysArgumentSetSPtr>
{
public:
    SysCommand(const SysExecutableSPtr& executable, const SysArgumentSetSPtr& arguments);

    const SysExecutableSPtr& executable() const
    {
        return std::get<0>(mArgs);
    }

    const SysArgumentSetSPtr& arguments() const
    {
        return std::get<1>(mArgs);
    }

    string toString() const;

private:
    mutable dp::SimpleMemoization<string> mCommandMemoization;
};
}
