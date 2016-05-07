//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/parse_stdout_task.h"
#include "task/manager.h"
#include "task/sys/execute_command_task.h"
#include "task/tpool.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <memory>

namespace task
{
std::function<ECode(int, const std::string&)> ParseStdoutTask::logOnError()
{
    return [](int exit, const std::string& stdout) -> ECode {
        if (exit != 0)
        {
            ELOG("\n{}", stdout);
            EHBan(kUnable);
        }
        EHEnd;
    };
}

ParseStdoutTask::ParseStdoutTask(const doim::SysCommandSPtr& command,
                                 const doim::FsDirectorySPtr& targetDirectory,
                                 rtti::ClassId parseId,
                                 std::function<ECode(int, const std::string&)> parse,
                                 const std::string& description)
    : Base(command, targetDirectory, parseId)
    , mParse(parse)
    , mDescription(description)
{
    ASSERT(doim::gManager->isUnique(command));
}

ECode ParseStdoutTask::operator()()
{
    auto executeTask = gManager->valid(
        std::make_shared<task::ExecuteCommandTask>(command(), targetDirectory()));
    gTPool->ensureScheduled(executeTask);
    EHTest(executeTask->join());

    std::string stdoutput;
    EHTest(executeTask->stdoutput(stdoutput));

    EHTest(mParse(executeTask->exit(), stdoutput));
    EHEnd;
}

std::string ParseStdoutTask::description() const
{
    return mDescription;
}

} // namespace task
