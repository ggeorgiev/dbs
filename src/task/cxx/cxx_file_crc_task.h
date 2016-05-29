//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
#include "task/crc_task.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxFileCrcTask;

typedef shared_ptr<CxxFileCrcTask> CxxFileCrcTaskSPtr;

class CxxFileCrcTask : public CxxCrcTaskMixin,
                       public CrcTask<CxxFileCrcTask, doim::CxxFileSPtr>
{
public:
    CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile);

    doim::CxxFileSPtr cxxFile() const
    {
        return std::get<0>(mArgs);
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    string description() const override;
};
}
