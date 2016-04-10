//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/cxx/cxx_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <cppformat/format.h>
#include <spdlog/details/line_logger_impl.h>
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class CxxFileCrcTask;

typedef std::shared_ptr<CxxFileCrcTask> CxxFileCrcTaskSPtr;

class CxxFileCrcTask : public CxxCrcTaskMixin,
                       public Base<CxxFileCrcTask, doim::CxxFileSPtr>
{
public:
    CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile);

    doim::CxxFileSPtr cxxFile() const
    {
        return std::get<0>(mArgs);
    }

    doim::TagSet tags() override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    std::string description() const override;
};
}
