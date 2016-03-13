//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "err/err.h"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class CxxHeaderCrcTask;

typedef std::shared_ptr<CxxHeaderCrcTask> CxxHeaderCrcTaskSPtr;

class CxxHeaderCrcTask
    : public CxxCrcTaskMixin,
      public Base<CxxHeaderCrcTask, doim::CxxHeaderSPtr, doim::CxxIncludeDirectorySPtr>
{
public:
    static CxxHeaderCrcTaskSPtr valid(const CxxHeaderCrcTaskSPtr& task);

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

    ECode operator()() override;
    std::string description() const override;
};
}
