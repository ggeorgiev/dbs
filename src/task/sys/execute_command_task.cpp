//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/execute_command_task.h"
#include "task/manager.h"
#include "task/sys/ensure_directory_task.h"
#include "task/tpool.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err_assert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <memory>
#include <string>
#include <stdio.h>
#include <sys/wait.h>

namespace task
{
ExecuteCommandTask::ExecuteCommandTask(const doim::SysCommandSPtr& command,
                                       const doim::FsDirectorySPtr& targetDirectory)
    : Base(command, targetDirectory)
{
    ASSERT(doim::gManager->isUnique(command));
    ASSERT(doim::gManager->isUnique(targetDirectory));
}

ECode ExecuteCommandTask::operator()()
{
    if (targetDirectory() != nullptr)
    {
        boost::filesystem::path path(targetDirectory()->path());

        if (!boost::filesystem::is_directory(path))
        {
            auto mkdirTask =
                task::gManager->valid(task::EnsureDirectoryTask::make(targetDirectory()));
            gTPool->ensureScheduled(mkdirTask);
            EHTest(mkdirTask->join());
        }
    }

    const auto& cmd = command()->string() + " 2>&1";

    auto pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        EHBan(kUnable, cmd);

    char buffer[1024];

    std::string stdoutput;
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

ECode ExecuteCommandTask::stdoutput(std::string& stdoutput) const
{
    ASSERT(finished());
    EHTest(db::gDatabase->get(stdoutputDbKey(), stdoutput));
    EHEnd;
}

std::string ExecuteCommandTask::stdoutputDbKey() const
{
    return "stdout: " + command()->string();
}

std::string ExecuteCommandTask::description() const
{
    return "System execute " + command()->string();
}

} // namespace task
