//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "tpool/task.hpp"
#include "doim/fs/fs_directory.h"
#include "err/err.h"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class EnsureDirectoryTask;

typedef std::shared_ptr<EnsureDirectoryTask> EnsureDirectoryTaskSPtr;

class EnsureDirectoryTask : public tpool::Task,
                            public Base<EnsureDirectoryTask, doim::FsDirectorySPtr>
{
public:
    EnsureDirectoryTask(const doim::FsDirectorySPtr& directory);

    doim::FsDirectorySPtr directory() const
    {
        return std::get<0>(mArgs);
    }

    ECode operator()() override;
    std::string description() const override;
};
}
