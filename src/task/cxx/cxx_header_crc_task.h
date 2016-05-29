//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
#include "task/crc_task.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxHeaderCrcTask;

typedef shared_ptr<CxxHeaderCrcTask> CxxHeaderCrcTaskSPtr;

class CxxHeaderCrcTask
    : public CxxCrcTaskMixin,
      public CrcTask<CxxHeaderCrcTask, doim::CxxHeaderSPtr, doim::CxxIncludeDirectorySPtr>
{
public:
    CxxHeaderCrcTask(const doim::CxxHeaderSPtr& cxxHeader,
                     const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory);

    doim::CxxHeaderSPtr cxxHeader() const
    {
        return std::get<0>(mArgs);
    }

    doim::CxxIncludeDirectorySPtr currentIncludeDirectory() const
    {
        return std::get<1>(mArgs);
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    string description() const override;
};
}
