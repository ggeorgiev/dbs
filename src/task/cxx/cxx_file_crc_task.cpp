//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_header.hpp"
#include <sstream>

namespace task
{
ECode CxxFileCrcTask::operator()()
{
    doim::CxxHeaderSet includes;
    EHTest(calculate<CxxHeaderCrcTask>(mCxxFile->file(),
                                       mCxxFile->cxxIncludeDirectories(),
                                       mCxxFile->cxxHeaders()));

    EHEnd;
}
} // namespace task
