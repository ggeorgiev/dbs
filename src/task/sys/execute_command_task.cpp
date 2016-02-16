//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/execute_command_task.h"
#include "err/err_cppformat.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <stdio.h>

namespace task
{
ECode ExecuteCommandTask::operator()()
{
    const auto& cmd = mCommand->string();

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
} // namespace task
