//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "task/manager.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <sstream>

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
    ASSERT(doim::gManager->isUnique(cxxHeader));
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

std::string CxxHeaderCrcTask::description() const
{
    return "Crc of " + cxxHeader()->file()->path();
}

} // namespace task
