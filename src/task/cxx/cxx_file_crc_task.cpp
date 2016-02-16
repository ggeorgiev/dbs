//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_header.h"
#include "doim/fs/fs_file.h"
#include "log/log.h"
#include <sstream>
#include <string>

namespace task
{
CxxFileCrcTask::CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile)
    : mCxxFile(cxxFile)
{
}

ECode CxxFileCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(mCxxFile->file(),
                                       nullptr,
                                       mCxxFile->cxxIncludeDirectories()));

    DLOG("Crc for {0} is {1:x}", mCxxFile->file()->name(), mCrcsum);
    EHEnd;
}

std::string CxxFileCrcTask::description() const
{
    return "Crc of " + mCxxFile->file()->path();
}

} // namespace task
