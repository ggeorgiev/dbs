//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/cxx/cxx_object_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <cppformat/format.h>
#include "math/crc.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class CxxObjectFileCrcTask;

typedef std::shared_ptr<CxxObjectFileCrcTask> CxxObjectFileCrcTaskSPtr;

class CxxObjectFileCrcTask : public Base<CxxObjectFileCrcTask, doim::CxxObjectFileSPtr>
{
public:
    CxxObjectFileCrcTask(const doim::CxxObjectFileSPtr& cxxObjectFile);

    doim::CxxObjectFileSPtr cxxObjectFile() const
    {
        return std::get<0>(mArgs);
    }

    math::Crcsum crc()
    {
        return mCrcsum;
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    std::string description() const override;

private:
    math::Crcsum mCrcsum;
};
}
