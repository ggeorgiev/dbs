//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/manager.h"
#include "task/sys/execute_command_task.h"
#include "tpool/task_callback.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err_assert.h"
#include "err/err_cppformat.h"
#include "log/log.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <stddef.h>
#include <stdio.h>

namespace task
{
tpool::TaskSPtr ExecuteCommandTask::createLogOnError(const doim::SysCommandSPtr& command,
                                                     const std::string& description)
{
    auto task =
        std::make_shared<task::ExecuteCommandTask>(command,
                                                   task::EExitExpectation::kNonZero,
                                                   description);

    task = task::gManager->valid(task);

    tpool::TaskCallback::Function onError = [](const tpool::TaskSPtr& task) -> ECode {
        const auto& executeTask = std::static_pointer_cast<ExecuteCommandTask>(task);

        std::string stdout;
        EHTest(executeTask->stdout(stdout));

        ELOG("{}", stdout);
        EHEnd;
    };
    return std::make_shared<tpool::TaskCallback>(0, task, nullptr, onError);
}

ExecuteCommandTask::ExecuteCommandTask(const doim::SysCommandSPtr& command,
                                       EExitExpectation exitExpectation,
                                       const std::string& description)
    : Base(command,
           static_cast<typename std::underlying_type<EExitExpectation>::type>(
               exitExpectation))
    , mDescription(description)
{
    ASSERT(doim::gManager->isUnique(command));
}

ECode ExecuteCommandTask::operator()()
{
    const auto& cmd = command()->string() + " 2>&1";

    auto pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        EHBan(kUnable, cmd);

    char buffer[1024];

    std::string stdout;
    while (!feof(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            stdout += buffer;
    }

    auto exit = pclose(pipe);

    EHTest(db::gDatabase->put(stdoutDbKey(), stdout));

    if (exitExpectation() == EExitExpectation::kNonZero && exit != 0)
        EHBan(kUnable, cmd);

    EHEnd;
}

ECode ExecuteCommandTask::stdout(std::string& stdout) const
{
    ASSERT(finished());
    EHTest(db::gDatabase->put(stdoutDbKey(), stdout));
    EHEnd;
}

std::string ExecuteCommandTask::stdoutDbKey() const
{
    return "stdout: " + command()->string();
}

std::string ExecuteCommandTask::description() const
{
    if (!mDescription.empty())
        return mDescription;
    return "Execute command " + command()->string();
}

} // namespace task
