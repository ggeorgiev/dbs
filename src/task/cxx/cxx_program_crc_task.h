//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/cxx/cxx_program.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxProgramCrcTask;
typedef shared_ptr<CxxProgramCrcTask> CxxProgramCrcTaskSPtr;

class CxxProgramCrcTask : public CrcTask<CxxProgramCrcTask, doim::CxxProgramSPtr>
{
public:
    using CrcTask<CxxProgramCrcTask, doim::CxxProgramSPtr>::CrcTask;

    bool check() const;

    doim::CxxProgramSPtr cxxProgram() const
    {
        return std::get<0>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;
};
}
