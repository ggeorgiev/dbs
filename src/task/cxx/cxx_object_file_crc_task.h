//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/cxx/cxx_object_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
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
    using CrcTask<CxxObjectFileCrcTask, doim::CxxObjectFileSPtr>::CrcTask;

    bool check() const;

    doim::CxxObjectFileSPtr cxxObjectFile() const
    {
        return std::get<0>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;
};
}
