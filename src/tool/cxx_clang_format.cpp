//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_clang_format.h"
#include "task/sys/parse_stdout_task.h"

#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/sys/sys_command.h"
#include "doim/set.hpp"
#include "rtti/class_rtti.hpp"
#include <memory>
#include <str>

namespace tool
{
doim::SysArgumentSPtr CxxClangFormat::gSortIncludesArgument =
    doim::SysArgument::global("-sort-includes", CxxClangFormat::gSortIncludesArgument);

CxxClangFormat::CxxClangFormat(const doim::SysExecutableSPtr& formatter)
    : mFormatter(formatter)
{
}

tpool::TaskSPtr CxxClangFormat::formatCommand(const doim::FsDirectorySPtr& directory,
                                              const doim::CxxFileSPtr& cxxFile) const
{
    auto arguments = doim::SysArgumentSet::make();
    arguments->insert(gSortIncludesArgument);

    const string& file = cxxFile->file()->path(directory);
    auto argument_i = doim::SysArgument::unique("-i " + file);
    arguments->insert(argument_i);

    auto formatCommand = doim::SysCommand::unique(mFormatter, arguments);

    auto id = rtti::RttiInfo<CxxClangFormat, 0>::classId();
    return task::ParseStdoutTask::valid(formatCommand,
                                        cxxFile->file()->directory(),
                                        id,
                                        task::ParseStdoutTask::logOnError(),
                                        "Format " + file);
}
}
