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

enum class EExitExpectation : unsigned char
{
    kNonZero,
    kAny
};

class ExecuteCommandTask
    : public Base<ExecuteCommandTask,
                  doim::SysCommandSPtr,
                  typename std::underlying_type<EExitExpectation>::type>
{
public:
    static tpool::TaskSPtr createLogOnError(const doim::SysCommandSPtr& command,
                                            const std::string& description);

    ExecuteCommandTask(const doim::SysCommandSPtr& command,
                       EExitExpectation exitExpectation,
                       const std::string& description);

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    EExitExpectation exitExpectation() const
    {
        return static_cast<EExitExpectation>(std::get<1>(mArgs));
    }

    ECode stdout(std::string& stdout) const;

    ECode operator()() override;
    std::string description() const override;

private:
    std::string stdoutDbKey() const;

    std::string mDescription;
};
}
