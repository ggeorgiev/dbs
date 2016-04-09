//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "tpool/task.h"
#include "doim/sys/command.h"
#include "err/err.h"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class ExecuteCommandTask;

typedef std::shared_ptr<ExecuteCommandTask> ExecuteCommandTaskSPtr;

class ExecuteCommandTask : public tpool::Task,
                           public Base<ExecuteCommandTask, doim::SysCommandSPtr>
{
public:
    ExecuteCommandTask(const doim::SysCommandSPtr& command,
                       const std::string description);

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    ECode operator()() override;
    std::string description() const override;

private:
    std::string mDescription;
};
}
