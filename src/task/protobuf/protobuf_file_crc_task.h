//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/protobuf/protobuf_file.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class ProtobufFileCrcTask;
typedef shared_ptr<ProtobufFileCrcTask> ProtobufFileCrcTaskSPtr;

class ProtobufFileCrcTask : public CrcTask<ProtobufFileCrcTask, doim::ProtobufFileSPtr>
{
public:
    ProtobufFileCrcTask(const doim::ProtobufFileSPtr& protobufFile);

    doim::ProtobufFileSPtr protobufFile() const
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
