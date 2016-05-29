//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_program_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/manager.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include "math/crc.hpp"
#include <algorithm>
#include <str>
#include <vector>

namespace task
{
CxxProgramCrcTask::CxxProgramCrcTask(const doim::CxxProgramSPtr& cxxProgram)
    : CrcTask(cxxProgram)
{
    ASSERT(cxxProgram->isUnique());
}

ECode CxxProgramCrcTask::operator()()
{
    const auto& objectFiles = cxxProgram()->cxxObjectFiles();

    std::vector<tpool::TaskSPtr> tasks;
    tasks.reserve(objectFiles->size());

    for (const auto& objectFile : *objectFiles)
    {
        tpool::TaskSPtr task;
        if (objectFile->source().type() == typeid(doim::CxxFileSPtr))
            task = gManager->valid(CxxFileCrcTask::make(objectFile->cxxFile()));
        else if (objectFile->source().type() == typeid(doim::CxxFileSPtr))
            task = gManager->valid(ProtobufFileCrcTask::make(objectFile->protobufFile()));
        else
            ASSERT(false);

        tasks.push_back(task);
    }

    auto group = tpool::TaskGroup::make(gTPool, 0, tasks);
    gTPool->ensureScheduled(group);
    EHTest(group->join());

    std::vector<math::Crcsum> crcs;
    crcs.reserve(objectFiles->size());

    for (const auto& task : group->tasks())
        crcs.push_back(std::static_pointer_cast<CrcTask>(task)->crc());

    std::sort(crcs.begin(), crcs.end());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(crcs.data(), sizeof(math::Crcsum) * crcs.size());
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

string CxxProgramCrcTask::description() const
{
    return "Cxx program crc " + cxxProgram()->file()->path();
}

} // namespace task
