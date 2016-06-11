//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/execute_command_task.h"
#include "task/sys/ensure_directory_task.h"
#include "task/tpool.h"
#include "db/database.h"
#include "err/err_assert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <memory>
#include <str>
#include <stdio.h>
#include <sys/wait.h>

namespace task
{
bool ExecuteCommandTask::check() const
{
    return command()->isUnique() && targetDirectory()->isUnique();
}

ECode ExecuteCommandTask::operator()()
{
    if (targetDirectory() != nullptr)
    {
        boost::filesystem::path path(targetDirectory()->path());

        if (!boost::filesystem::is_directory(path))
        {
            auto mkdirTask = task::EnsureDirectoryTask::valid(targetDirectory());
            gTPool->ensureScheduled(mkdirTask);
            EHTest(mkdirTask->join());
        }
    }

    const auto& cmd = command()->toString(nullptr) + " 2>&1";

    auto pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        EHBan(kUnable, cmd);

    char buffer[1024];

    string stdoutput;
    while (!feof(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            stdoutput += buffer;
    }
    int exit = pclose(pipe);
    mExit = WEXITSTATUS(exit);

    EHTest(db::gDatabase->put(stdoutputDbKey(), stdoutput));
    EHEnd;
}

int ExecuteCommandTask::exit() const
{
    ASSERT(finished());
    return mExit;
}

ECode ExecuteCommandTask::stdoutput(string& stdoutput) const
{
    ASSERT(finished());
    EHTest(db::gDatabase->get(stdoutputDbKey(), stdoutput));
    EHEnd;
}

string ExecuteCommandTask::stdoutputDbKey() const
{
    return "stdout: " + command()->toString(nullptr);
}

string ExecuteCommandTask::description() const
{
    return "System execute " + command()->toString(nullptr);
}

} // namespace task
