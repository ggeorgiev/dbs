//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/manager.h"
#include "task/sys/ensure_directory_task.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include "err/err_cppformat.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <sstream>
#include <string>

namespace task
{
EnsureDirectoryTask::EnsureDirectoryTask(const doim::FsDirectorySPtr& directory)
    : Base(directory)
{
    ASSERT(doim::gManager->isUnique(directory));
}

ECode EnsureDirectoryTask::operator()()
{
    boost::filesystem::path path(directory()->path());

    if (boost::filesystem::is_directory(path))
        EHEnd;

    if (directory()->parent() != nullptr)
    {
        auto mkdirTask = task::gManager->valid(
            std::make_shared<task::EnsureDirectoryTask>(directory()->parent()));
        EHTest(mkdirTask->join());
    }

    if (!boost::filesystem::create_directory(path))
        if (!boost::filesystem::is_directory(path))
            EHBan(kFileSystem);
    EHEnd;
}

std::string EnsureDirectoryTask::description() const
{
    return "Ensure directory " + directory()->path();
}

} // namespace task
