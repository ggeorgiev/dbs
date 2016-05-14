//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/sys/command.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class ExecuteCommandTask;
typedef shared_ptr<ExecuteCommandTask> ExecuteCommandTaskSPtr;

class ExecuteCommandTask
    : public Base<ExecuteCommandTask, doim::SysCommandSPtr, doim::FsDirectorySPtr>
{
public:
    ExecuteCommandTask(const doim::SysCommandSPtr& command,
                       const doim::FsDirectorySPtr& targetDirectory);

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    doim::FsDirectorySPtr targetDirectory() const
    {
        return std::get<1>(mArgs);
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gSysTag};
    }

    int exit() const;
    ECode stdoutput(string& stdoutput) const;

    ECode operator()() override;
    string description() const override;

private:
    int mExit;
    string stdoutputDbKey() const;
};
}
