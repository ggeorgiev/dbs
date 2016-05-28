//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/cxx/cxx_program.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include "math/crc.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxProgramCrcTask;

typedef shared_ptr<CxxProgramCrcTask> CxxProgramCrcTaskSPtr;

class CxxProgramCrcTask : public Element<CxxProgramCrcTask, doim::CxxProgramSPtr>
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

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    string description() const override;

private:
    math::Crcsum mCrcsum;
};
}
