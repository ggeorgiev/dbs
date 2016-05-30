//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_executable.h"
#include "doim/element.hpp"
#include "dp/simple_memoization.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class SysCommand;
typedef shared_ptr<SysCommand> SysCommandSPtr;

class SysCommand
    : public Element<SysCommand, FsDirectorySPtr, SysExecutableSPtr, SysArgumentSetSPtr>
{
public:
    SysCommand(const FsDirectorySPtr& directory,
               const SysExecutableSPtr& executable,
               const SysArgumentSetSPtr& arguments);

    const FsDirectorySPtr& directory() const
    {
        return std::get<0>(mArgs);
    }

    const SysExecutableSPtr& executable() const
    {
        return std::get<1>(mArgs);
    }

    const SysArgumentSetSPtr& arguments() const
    {
        return std::get<2>(mArgs);
    }

    string toString(const FsDirectorySPtr& root) const;
};
}
