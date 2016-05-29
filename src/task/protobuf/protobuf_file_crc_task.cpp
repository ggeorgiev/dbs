//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/protobuf/protobuf_file_crc_task.h"
#include "doim/fs/fs_file.h"
#include "err/err_assert.h"
#include "log/log.h"
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>

namespace task
{
ProtobufFileCrcTask::ProtobufFileCrcTask(const doim::ProtobufFileSPtr& protobufFile)
    : CrcTask(protobufFile)
{
    ASSERT(protobufFile->isUnique());
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
