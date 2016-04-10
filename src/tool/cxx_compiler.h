//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "task/sys/ensure_directory_task.h"
#include "task/sys/execute_command_task.h"
#include "dom/cxx/cxx_program.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.h"
#include "db/database.h"
#include <memory>
#include <set>
#include <sstream>
#include <string>

namespace tool
{
class CxxCompiler;
typedef std::shared_ptr<CxxCompiler> CxxCompilerSPtr;

class CxxCompiler
{
public:
    CxxCompiler(const doim::SysExecutableSPtr& compiler);

    doim::SysArgumentSetSPtr includeArguments(
        const doim::FsDirectorySPtr& directory,
        const doim::CxxIncludeDirectorySetSPtr& includeDirectories);

    ECode tasks(const doim::FsDirectorySPtr& directory,
                const doim::CxxObjectFileSPtr& objectFile,
                std::vector<tpool::TaskSPtr>& tasks);

    ECode commands(const doim::FsDirectorySPtr& directory,
                   const dom::CxxProgramSPtr& program,
                   std::string& cmd);

private:
    doim::SysExecutableSPtr mCompiler;
};
}
