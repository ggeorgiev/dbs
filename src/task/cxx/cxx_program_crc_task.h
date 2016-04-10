//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/cxx/cxx_program.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <cppformat/format.h>
#include <spdlog/details/line_logger_impl.h>
#include "math/crc.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class CxxProgramCrcTask;

typedef std::shared_ptr<CxxProgramCrcTask> CxxProgramCrcTaskSPtr;

class CxxProgramCrcTask : public Base<CxxProgramCrcTask, doim::CxxProgramSPtr>
{
public:
    CxxProgramCrcTask(const doim::CxxProgramSPtr& cxxProgram);

    doim::CxxProgramSPtr cxxProgram() const
    {
        return std::get<0>(mArgs);
    }

    math::Crcsum crc() const
    {
        return mCrcsum;
    }

    doim::TagSet tags() override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    std::string description() const override;

private:
    math::Crcsum mCrcsum;
};
}
