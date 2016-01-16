//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_header_crc_task.h"
#include "task/manager.h"
#include "doim/cxx/cxx_header.hpp"
#include <sstream>

namespace task
{
ECode CxxFileCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate(mCxxFile->file(),
                     mCxxFile->cxxIncludeDirectories(),
                     mCxxFile->cxxHeaders(),
                     includes));

    for (const auto& header : includes)
    {
        if (header->type() == doim::CxxHeader::Type::kSystem)
            continue;

        auto task = std::make_shared<CxxHeaderCrcTask>(header, mCxxFile->cxxHeaders());
        task = gManager->valid(task);
        EHTest(task->join());
        mCrc64 = mCrc64 ^ task->crc();
    }

    EHEnd;
}
} // namespace task
