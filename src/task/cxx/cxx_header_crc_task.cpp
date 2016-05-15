//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "task/manager.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <str>

namespace task
{
CxxHeaderCrcTaskSPtr CxxHeaderCrcTask::valid(const CxxHeaderCrcTaskSPtr& task)
{
    return gManager->valid(task);
}

CxxHeaderCrcTask::CxxHeaderCrcTask(
    const doim::CxxHeaderSPtr& cxxHeader,
    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory)
    : Base(cxxHeader, currentIncludeDirectory)
{
    ASSERT(cxxHeader->isUnique());
    ASSERT(doim::gManager->isUnique(currentIncludeDirectory));
}

ECode CxxHeaderCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(cxxHeader()->file(),
                                       currentIncludeDirectory(),
                                       cxxHeader()->cxxIncludeDirectories()));
    EHEnd;
}

string CxxHeaderCrcTask::description() const
{
    return "Cxx header file crc " + cxxHeader()->file()->path();
}

} // namespace task
