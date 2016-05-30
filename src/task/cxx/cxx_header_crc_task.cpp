//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "doim/fs/fs_file.h"
#include "err/err_assert.h"
#include <str>

namespace task
{
CxxHeaderCrcTask::CxxHeaderCrcTask(
    const doim::CxxHeaderSPtr& cxxHeader,
    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory)
    : CrcTask(cxxHeader, currentIncludeDirectory)
{
    ASSERT(cxxHeader->isUnique());
    ASSERT(currentIncludeDirectory->isUnique());
}

ECode CxxHeaderCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(cxxHeader(), currentIncludeDirectory(), mCrcsum));
    EHEnd;
}

string CxxHeaderCrcTask::description() const
{
    return "Cxx header file crc " + cxxHeader()->file()->path();
}

} // namespace task
