//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"

#include <sstream>

#include "task/manager.h"

namespace task
{
ECode CxxHeaderCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate(mCxxHeader->file(),
                     mCxxHeader->cxxIncludeDirectories(),
                     mCxxHeaders,
                     includes));

    for (const auto& header : includes)
    {
        if (header->type() == doim::CxxHeader::Type::kSystem)
            continue;

        auto task = std::make_shared<CxxHeaderCrcTask>(header, mCxxHeaders);
        task = gManager->valid(task);
        EHTest(task->join());
        mCrc64 = mCrc64 ^ task->crc();
    }
    EHEnd;
}
} // namespace task
