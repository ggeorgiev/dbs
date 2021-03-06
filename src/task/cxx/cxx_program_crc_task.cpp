//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_program_crc_task.h"
#include "task/cxx/cxx_source_crc_task.h"
#include "task/tpool.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include "math/crc.hpp"
#include <str>
#include <unordered>
#include <utility>
#include <vector>

namespace task
{
bool CxxProgramCrcTask::check() const
{
    return cxxProgram() != nullptr && cxxProgram()->isUnique();
}

static doim::TagSetSPtr gTags = doim::TagSet::global(
    {&doim::gCrcTag, &doim::gInspectTag, &doim::gCxxTag, &doim::gProgramTag}, gTags);

doim::TagSetSPtr CxxProgramCrcTask::tags() const
{
    return gTags;
}

ECode CxxProgramCrcTask::operator()()
{
    const auto& objectFiles = cxxProgram()->cxxObjectFiles();

    std::vector<CxxSourceCrcTaskSPtr> tasks;
    tasks.reserve(objectFiles->size());

    for (const auto& objectFile : *objectFiles)
    {
        auto cxxTask = CxxSourceCrcTask::valid(CxxSourceCrcTask::EDepth::kAll,
                                               objectFile->cxxFile(),
                                               nullptr);
        tasks.push_back(cxxTask);
    }

    auto group = tpool::TaskGroup::make(gTPool, 0, tasks);
    gTPool->ensureScheduled(group);
    EHTest(group->join());

    unordered_set<math::Crcsum> crcs;

    math::Crcsum x = 0;
    for (const auto& task : tasks)
    {
        auto n = task->crc();
        if (n == 0)
        {
            mCrcsum = 0;
            EHEnd;
        }
        auto unique = crcs.insert(n).second;
        if (!unique)
            EHBan(kTooMany, "There are at least two items with the same crc");
        x ^= n;
    }

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(&x, sizeof(x));
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

string CxxProgramCrcTask::description() const
{
    return "Cxx program crc " + cxxProgram()->file()->path();
}

} // namespace task
