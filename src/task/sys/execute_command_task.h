//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "doim/sys/command.hpp"
#include "err/err.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <stddef.h>

namespace task
{
class ExecuteCommandTask;

typedef std::shared_ptr<ExecuteCommandTask> ExecuteCommandTaskSPtr;

class ExecuteCommandTask : public tpool::Task
{
public:
    ExecuteCommandTask(const doim::SysCommandSPtr& command, const std::string description)
        : tpool::Task(0)
        , mDescription(description)
        , mCommand(command)
    {
    }

    ECode operator()() override;

    std::string description() const override
    {
        if (!mDescription.empty())
            return mDescription;
        return "Execute command " + mCommand->string();
    }

    struct Hasher
    {
        std::size_t operator()(const ExecuteCommandTaskSPtr& task) const
        {
            return hashSC(task->mCommand);
        }

        bool operator()(const ExecuteCommandTaskSPtr& task1,
                        const ExecuteCommandTaskSPtr& task2) const
        {
            return task1->mCommand == task2->mCommand;
        }

        std::hash<doim::SysCommandSPtr> hashSC;
    };

private:
    std::string mDescription;
    doim::SysCommandSPtr mCommand;
};
}
