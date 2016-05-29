//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/protobuf/protobuf_file_crc_task.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "err/err_assert.h"
#include "log/log.h"
#include <boost/filesystem/operations.hpp>
#include <boost/type_index.hpp>
#include <iterator>
#include <str>

namespace task
{
CxxObjectFileCrcTask::CxxObjectFileCrcTask(const doim::CxxObjectFileSPtr& cxxObjectFile)
    : CrcTask(cxxObjectFile)
{
    ASSERT(cxxObjectFile->isUnique());
}

ECode CxxObjectFileCrcTask::operator()()
{
    const auto& path = cxxObjectFile()->file()->path(nullptr);
    if (!boost::filesystem::exists(path))
    {
        mCrcsum = 0;
        EHEnd;
    }

    ICrcTaskSPtr crcTask;
    if (cxxObjectFile()->source().type() == typeid(doim::CxxFileSPtr))
    {
        auto cxxTask = CxxFileCrcTask::valid(cxxObjectFile()->cxxFile());
        task::gTPool->ensureScheduled(cxxTask);
        crcTask = cxxTask;
    }
    else if (cxxObjectFile()->source().type() == typeid(doim::CxxFileSPtr))
    {
        auto protobufTask = ProtobufFileCrcTask::valid(cxxObjectFile()->protobufFile());
        task::gTPool->ensureScheduled(protobufTask);
        crcTask = protobufTask;
    }
    else
        ASSERT(false);

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());

    math::Crcsum crc;
    EHTest(crcTask->crc(crc));
    if (crc == 0)
    {
        mCrcsum = 0;
        EHEnd;
    }
    crcProcessor.process_bytes(&crc, sizeof(crc));
    mCrcsum = crcProcessor.checksum();

    DLOG("Crc for {0} is {1:x}", cxxObjectFile()->file()->name(), mCrcsum);
    EHEnd;
}

string CxxObjectFileCrcTask::description() const
{
    return "Cxx object file crc " + cxxObjectFile()->file()->path();
}

} // namespace task
