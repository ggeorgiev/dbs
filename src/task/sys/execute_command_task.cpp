//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/execute_command_task.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err_assert.h"
#include "err/err_cppformat.h"
#include <memory>
#include <string>
#include <stdio.h>
#include <sys/wait.h>

namespace task
{
ExecuteCommandTask::ExecuteCommandTask(const doim::SysCommandSPtr& command)
    : Base(command)
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
    int exit = pclose(pipe);
    mExit = WEXITSTATUS(exit);

    EHTest(db::gDatabase->put(stdoutDbKey(), stdout));
    EHEnd;
}

int ExecuteCommandTask::exit() const
{
    ASSERT(finished());
    return mExit;
}

ECode ExecuteCommandTask::stdout(std::string& stdout) const
{
    ASSERT(finished());
    EHTest(db::gDatabase->get(stdoutDbKey(), stdout));
    EHEnd;
}

std::string ExecuteCommandTask::stdoutDbKey() const
{
    return "stdout: " + command()->string();
}

std::string ExecuteCommandTask::description() const
{
    return "System execute " + command()->string();
}

} // namespace task
