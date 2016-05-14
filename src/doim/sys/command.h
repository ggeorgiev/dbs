//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/sys/executable.h"
#include "dp/simple_memoization.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class SysArgument;
typedef std::shared_ptr<SysArgument> SysArgumentSPtr;
typedef SetBase<SysArgumentSPtr> SysArgumentSet;
typedef std::shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

class SysCommand;
typedef std::shared_ptr<SysCommand> SysCommandSPtr;

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

    std::string string() const;

private:
    mutable dp::SimpleMemoization<std::string> mCommandMemoization;
};
}
