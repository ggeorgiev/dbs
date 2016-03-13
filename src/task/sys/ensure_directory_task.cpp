//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/ensure_directory_task.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <string>
#include <stdlib.h>

namespace task
{
EnsureDirectoryTask::EnsureDirectoryTask(const doim::FsDirectorySPtr& directory)
    : tpool::Task(0)
    , Base(directory)
{
    ASSERT(doim::gManager->isUnique(directory));
}

ECode EnsureDirectoryTask::operator()()
{
    std::string cmd = "mkdir -p " + directory()->path();
    system(cmd.c_str());
    EHEnd;
}

std::string EnsureDirectoryTask::description() const
{
    return "Ensure directory " + directory()->path();
}

} // namespace task
