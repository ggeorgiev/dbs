//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/manager.h"
#include "task/tpool.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <iterator>
#include <string>

namespace task
{
CxxObjectFileCrcTask::CxxObjectFileCrcTask(const doim::CxxObjectFileSPtr& cxxObjectFile)
    : Base(cxxObjectFile)
{
    ASSERT(doim::gManager->isUnique(cxxObjectFile));
}

ECode CxxObjectFileCrcTask::operator()()
{
    auto task =
        gManager->valid(std::make_shared<CxxFileCrcTask>(cxxObjectFile()->cxxFile()));
    task::gTPool->ensureScheduled(task);

    std::ifstream fstream(cxxObjectFile()->file()->path(nullptr).c_str());
    std::string content((std::istreambuf_iterator<char>(fstream)),
                        std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());

    EHTest(task->join());

    auto crc = task->crc();
    crcProcessor.process_bytes(&crc, sizeof(crc));
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

std::string CxxObjectFileCrcTask::description() const
{
    return "Cxx object file crc " + cxxObjectFile()->file()->path();
}

} // namespace task
