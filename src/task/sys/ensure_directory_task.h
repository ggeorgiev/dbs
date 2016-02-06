//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "doim/fs/fs_directory.hpp"
#include "err/err.h"
#include <memory>
#include <stddef.h>

namespace task
{
class EnsureDirectoryTask;
typedef std::shared_ptr<EnsureDirectoryTask> EnsureDirectoryTaskSPtr;

class EnsureDirectoryTask : public tpool::Task
{
public:
    EnsureDirectoryTask(const doim::FsDirectorySPtr& directory)
        : tpool::Task(0)
        , mDirectory(directory)
    {
    }

    ECode operator()() override;

    std::string description() const override
    {
        return "Ensure directory " + mDirectory->path();
    }

    struct Hasher
    {
        std::size_t operator()(const EnsureDirectoryTaskSPtr& task) const
        {
            return hashFD(task->mDirectory);
        }

        bool operator()(const EnsureDirectoryTaskSPtr& task1,
                        const EnsureDirectoryTaskSPtr& task2) const
        {
            return task1->mDirectory == task2->mDirectory;
        }

        std::hash<doim::FsDirectorySPtr> hashFD;
    };

private:
    doim::FsDirectorySPtr mDirectory;
};
}
