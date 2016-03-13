//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/execute_command_task.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include "err/err_cppformat.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <stddef.h>
#include <stdio.h>

namespace task
{
ExecuteCommandTask::ExecuteCommandTask(const doim::SysCommandSPtr& command,
                                       const std::string description)
    : tpool::Task(0)
    , Base(command)
    , mDescription(description)
{
    ASSERT(doim::gManager->isUnique(command));
}

ECode ExecuteCommandTask::operator()()
{
    const auto& cmd = command()->string();

    auto pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        EHBan(kUnable, cmd);

    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    std::cout << result;
    if (pclose(pipe) != 0)
        EHBan(kUnable, cmd);

    EHEnd;
}

std::string ExecuteCommandTask::description() const
{
    if (!mDescription.empty())
        return mDescription;
    return "Execute command " + command()->string();
}

} // namespace task
