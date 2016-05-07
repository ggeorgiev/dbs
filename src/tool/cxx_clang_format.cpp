//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_clang_format.h"
#include "task/manager.h"
#include "task/sys/parse_stdout_task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/command.h"
#include "err/err.h"
#include "rtti/class_rtti.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tool
{
CxxClangFormat::CxxClangFormat(const doim::SysExecutableSPtr& formatter)
    : mFormatter(formatter)
{
}

tpool::TaskSPtr CxxClangFormat::formatCommand(const doim::FsDirectorySPtr& directory,
                                              const doim::CxxFileSPtr& cxxFile) const
{
    auto arguments = std::make_shared<doim::SysArgumentSet>();

    auto argument_sort_includes = doim::gManager->obtainArgument("-sort-includes");
    arguments->insert(argument_sort_includes);

    const std::string& file = cxxFile->file()->path(directory);
    auto argument_i = doim::gManager->obtainArgument("-i " + file);
    arguments->insert(argument_i);

    auto formatCommand = std::make_shared<doim::SysCommand>(mFormatter, arguments);
    formatCommand = doim::gManager->unique(formatCommand);

    auto id = rtti::RttiInfo<CxxClangFormat, 0>::classId();
    return task::gManager->valid(
        std::make_shared<task::ParseStdoutTask>(formatCommand,
                                                cxxFile->file()->directory(),
                                                id,
                                                task::ParseStdoutTask::logOnError(),
                                                "Compile " + file));
}
}
