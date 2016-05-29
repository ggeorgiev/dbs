//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/manager.h"
#include "task/tpool.h"
#include "doim/fs/fs_file.h"

#include "err/err_assert.h"
#include "log/log.h"
#include <boost/filesystem/operations.hpp>
#include <iterator>
#include <str>

namespace task
{
CxxObjectFileCrcTask::CxxObjectFileCrcTask(const doim::CxxObjectFileSPtr& cxxObjectFile)
    : Element(cxxObjectFile)
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

    boost::variant<CxxFileCrcTaskSPtr, ProtobufFileCrcTaskSPtr> task;
    if (cxxObjectFile()->source().type() == typeid(doim::CxxFileSPtr))
        task = gManager->valid(CxxFileCrcTask::make(cxxObjectFile()->cxxFile()));
    else if (cxxObjectFile()->source().type() == typeid(doim::CxxFileSPtr))
        task =
            gManager->valid(ProtobufFileCrcTask::make(cxxObjectFile()->protobufFile()));
    else
        ASSERT(false);
    boost::apply_visitor(
        [](auto const& task) { return task::gTPool->ensureScheduled(task); }, task);

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());

    EHTest(boost::apply_visitor([](auto const& task) { EHTest(task->join()); }, task));

    auto crc = boost::apply_visitor([](auto const& task) { return task->crc(); }, task);
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
