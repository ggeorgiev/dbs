//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "task/manager.h"
#include <sstream>

namespace task
{
CxxHeaderCrcTaskSPtr CxxHeaderCrcTask::valid(const CxxHeaderCrcTaskSPtr& task)
{
    return gManager->valid(task);
}

ECode CxxHeaderCrcTask::operator()()
{
    Crc crc;

    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(mCxxHeader->file(),
                                       mCurrentIncludeDirectory,
                                       mCxxHeader->cxxIncludeDirectories()));
    EHEnd;
}
} // namespace task
