//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/parse_stdout_task.h"
#include "task/manager.h"
#include "task/sys/execute_command_task.h"
#include "task/tpool.h"

#include "err/err_assert.h"
#include "log/log.h"
#include <memory>

namespace task
{
std::function<ECode(int, const string&)> ParseStdoutTask::logOnError()
{
    return [](int exit, const string& stdoutput) -> ECode {
        if (exit != 0)
        {
            ELOG("\n{}", stdoutput);
            EHBan(kUnable);
        }
        EHEnd;
    };
}

ParseStdoutTask::ParseStdoutTask(const doim::SysCommandSPtr& command,
                                 const doim::FsDirectorySPtr& targetDirectory,
                                 rtti::ClassId parseId,
                                 std::function<ECode(int, const string&)> parse,
                                 const string& description)
    : Base(command, targetDirectory, parseId)
    , mParse(parse)
    , mDescription(description)
{
    ASSERT(command->isUnique());
}

ECode ParseStdoutTask::operator()()
{
    auto executeTask =
        gManager->valid(task::ExecuteCommandTask::make(command(), targetDirectory()));
    gTPool->ensureScheduled(executeTask);
    EHTest(executeTask->join());

    string stdoutput;
    EHTest(executeTask->stdoutput(stdoutput));

    EHTest(mParse(executeTask->exit(), stdoutput));
    EHEnd;
}

string ParseStdoutTask::description() const
{
    return mDescription;
}

} // namespace task
