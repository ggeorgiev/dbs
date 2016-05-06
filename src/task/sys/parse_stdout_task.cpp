//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/parse_stdout_task.h"
#include "task/manager.h"
#include "task/sys/execute_command_task.h"
#include "task/tpool.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <memory>
#include <stdio.h>

namespace task
{
ParseStdoutTask::ParseStdoutTask(const doim::SysCommandSPtr& command,
                                 rtti::ClassId parseId,
                                 std::function<ECode(int, const std::string&)> parse,
                                 const std::string& description)
    : Base(command, parseId)
    , mParse(parse)
    , mDescription(description)
{
    ASSERT(doim::gManager->isUnique(command));
}

ECode ParseStdoutTask::operator()()
{
    auto executeTask =
        gManager->valid(std::make_shared<task::ExecuteCommandTask>(command()));
    gTPool->ensureScheduled(executeTask);
    EHTest(executeTask->join());

    std::string stdout;
    EHTest(executeTask->stdout(stdout));

    EHTest(mParse(executeTask->exit(), stdout));
    EHEnd;
}

std::string ParseStdoutTask::description() const
{
    return mDescription;
}

} // namespace task
