//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/fs/fs_directory.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class EnsureDirectoryTask;

typedef shared_ptr<EnsureDirectoryTask> EnsureDirectoryTaskSPtr;

class EnsureDirectoryTask : public Element<EnsureDirectoryTask, doim::FsDirectorySPtr>
{
public:
    using Element<EnsureDirectoryTask, doim::FsDirectorySPtr>::Element;

    bool check() const;

    doim::FsDirectorySPtr directory() const
    {
        return std::get<0>(mArgs);
    }

    ECode operator()() override;
    string description() const override;
};
}
