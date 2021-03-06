//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/protobuf/protobuf_file_crc_task.h"
#include "doim/fs/fs_file.h"
#include "err/err_assert.h"
#include "log/log.h"
#include "math/crc.hpp"
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>

namespace task
{
bool ProtobufFileCrcTask::check() const
{
    return protobufFile() != nullptr && protobufFile()->isUnique();
}

static doim::TagSetSPtr gTags =
    doim::TagSet::global({&doim::gCrcTag, &doim::gInspectTag, &doim::gProtobufTag},
                         gTags);

doim::TagSetSPtr ProtobufFileCrcTask::tags() const
{
    return gTags;
}

ECode ProtobufFileCrcTask::operator()()
{
    std::ifstream fstream(protobufFile()->file()->path().c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());
    mCrcsum = crcProcessor.checksum();

    DLOG("Crc for {0} is {1:x}", protobufFile()->file()->name(), mCrcsum);
    EHEnd;
}

string ProtobufFileCrcTask::description() const
{
    return "Cxx file crc " + protobufFile()->file()->path();
}

} // namespace task
