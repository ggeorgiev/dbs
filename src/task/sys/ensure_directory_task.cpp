//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/ensure_directory_task.h"
#include "task/tpool.h"
#include "err/err_assert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <str>

namespace task
{
bool EnsureDirectoryTask::check() const
{
    return directory()->isUnique();
}

ECode EnsureDirectoryTask::operator()()
{
    if (directory()->parent() != nullptr)
    {
        boost::filesystem::path path(directory()->parent()->path());
        if (!boost::filesystem::is_directory(path))
        {
            auto mkdirTask = task::EnsureDirectoryTask::valid(directory()->parent());
            task::gTPool->ensureScheduled(mkdirTask);
            EHTest(mkdirTask->join());
        }
    }

    boost::filesystem::path path(directory()->path());
    if (!boost::filesystem::create_directory(path))
        if (!boost::filesystem::is_directory(path))
            EHBan(kFileSystem);
    EHEnd;
}

string EnsureDirectoryTask::description() const
{
    return "Ensure directory " + directory()->path();
}

} // namespace task
