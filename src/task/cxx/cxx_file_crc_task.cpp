//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_header.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include "log/log.h"
#include <str>

namespace task
{
CxxFileCrcTask::CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile)
    : Base(cxxFile)
{
    ASSERT(cxxFile->isUnique());
}

ECode CxxFileCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(cxxFile()->file(),
                                       nullptr,
                                       cxxFile()->cxxIncludeDirectories()));

    DLOG("Crc for {0} is {1:x}", cxxFile()->file()->name(), mCrcsum);
    EHEnd;
}

string CxxFileCrcTask::description() const
{
    return "Cxx file crc " + cxxFile()->file()->path();
}

} // namespace task
