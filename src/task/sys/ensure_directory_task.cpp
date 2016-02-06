//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/sys/ensure_directory_task.h"
#include "task/manager.h"
#include "db/database.h"
#include <sstream>
#include <stdlib.h>

namespace task
{
ECode EnsureDirectoryTask::operator()()
{
    std::string cmd = "mkdir -p " + mDirectory->path();
    system(cmd.c_str());
    EHEnd;
}
} // namespace task
