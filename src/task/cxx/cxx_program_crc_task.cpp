//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_program_crc_task.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/manager.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace task
{
CxxProgramCrcTask::CxxProgramCrcTask(const doim::CxxProgramSPtr& cxxProgram)
    : tpool::Task(0)
    , mCxxProgram(cxxProgram)
{
}

ECode CxxProgramCrcTask::operator()()
{
    const auto& objectFiles = mCxxProgram->cxxObjectFiles();

    std::vector<math::Crcsum> crcs;
    crcs.reserve(objectFiles->size());

    for (const auto& objectFile : *objectFiles)
    {
        auto task =
            gManager->valid(std::make_shared<CxxFileCrcTask>(objectFile->cxxFile()));
        EHTest(task->join());
        crcs.push_back(task->crc());
    }

    std::sort(crcs.begin(), crcs.end());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(crcs.data(), sizeof(math::Crcsum) * crcs.size());
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

std::string CxxProgramCrcTask::description() const
{
    return "Crc of " + mCxxProgram->file()->path();
}

} // namespace task
