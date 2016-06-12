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
    using CrcTask<ProtobufFileCrcTask, doim::ProtobufFileSPtr>::CrcTask;

    bool check() const;

    doim::ProtobufFileSPtr protobufFile() const
    {
        return std::get<0>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;
};
}
