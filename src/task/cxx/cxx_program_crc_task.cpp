//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_program_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/protobuf/protobuf_file_crc_task.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include "math/crc.hpp"
#include <boost/type_index.hpp>
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

    std::vector<ICrcTaskSPtr> crcTasks;
    crcTasks.reserve(objectFiles->size());

    std::vector<tpool::TaskSPtr> tasks;
    tasks.reserve(objectFiles->size());

    for (const auto& objectFile : *objectFiles)
    {
        if (objectFile->source().type() == typeid(doim::CxxFileSPtr))
        {
            auto cxxTask = CxxFileCrcTask::valid(objectFile->cxxFile());
            tasks.push_back(cxxTask);
            crcTasks.push_back(cxxTask);
        }
        else if (objectFile->source().type() == typeid(doim::CxxFileSPtr))
        {
            auto protobufTask = ProtobufFileCrcTask::valid(objectFile->protobufFile());
            tasks.push_back(protobufTask);
            crcTasks.push_back(protobufTask);
        }
        else
            ASSERT(false);
    }

    auto group = tpool::TaskGroup::make(gTPool, 0, tasks);
    gTPool->ensureScheduled(group);
    EHTest(group->join());

    std::vector<math::Crcsum> crcs;
    crcs.reserve(objectFiles->size());

    for (const auto& task : crcTasks)
        crcs.push_back(task->crc());

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
