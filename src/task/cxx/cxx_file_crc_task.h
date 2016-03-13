//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/cxx/cxx_file.h"
#include "err/err.h"
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

    ECode operator()() override;
    std::string description() const override;
};
}
