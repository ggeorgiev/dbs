//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/sys/command.h"
#include "err/err.h"
#include <cppformat/format.h>
#include <spdlog/details/line_logger_impl.h>
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class ExecuteCommandTask;

typedef std::shared_ptr<ExecuteCommandTask> ExecuteCommandTaskSPtr;

class ExecuteCommandTask : public Base<ExecuteCommandTask, doim::SysCommandSPtr>
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
