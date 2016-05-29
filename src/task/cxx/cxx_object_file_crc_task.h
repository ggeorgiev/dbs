//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/cxx/cxx_object_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include "math/crc.hpp"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxObjectFileCrcTask;
typedef shared_ptr<CxxObjectFileCrcTask> CxxObjectFileCrcTaskSPtr;

class CxxObjectFileCrcTask : public CrcTask<CxxObjectFileCrcTask, doim::CxxObjectFileSPtr>
{
public:
    CxxObjectFileCrcTask(const doim::CxxObjectFileSPtr& cxxObjectFile);

    doim::CxxObjectFileSPtr cxxObjectFile() const
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
