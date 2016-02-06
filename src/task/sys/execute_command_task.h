//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "doim/sys/command.hpp"
#include "err/err.h"
#include <memory>
#include <stddef.h>

namespace task
{
class ExecuteCommandTask;
typedef std::shared_ptr<ExecuteCommandTask> ExecuteCommandTaskSPtr;

class ExecuteCommandTask : public tpool::Task
{
public:
    ExecuteCommandTask(const doim::SysCommandSPtr& command)
        : tpool::Task(0)
        , mCommand(command)
    {
    }

    ECode operator()() override;

    std::string description() const override
    {
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
    doim::SysCommandSPtr mCommand;
};
}
