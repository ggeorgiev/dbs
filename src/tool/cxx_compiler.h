//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/command.h"
#include "doim/sys/executable.hpp"
#include <memory>

namespace tool
{
class CxxCompiler;
typedef std::shared_ptr<CxxCompiler> CxxCompilerSPtr;

class CxxCompiler : public std::enable_shared_from_this<CxxCompiler>
{
public:
    static doim::SysArgumentSetSPtr includeArguments(
        const doim::FsDirectorySPtr& directory,
        const doim::CxxIncludeDirectorySetSPtr& includeDirectories);

    CxxCompiler(const doim::SysExecutableSPtr& compiler);

    tpool::TaskSPtr compileCommand(const doim::FsDirectorySPtr& directory,
                                   const doim::CxxObjectFileSPtr& objectFile) const;

    tpool::TaskSPtr linkCommand(const doim::FsDirectorySPtr& directory,
                                const doim::FsDirectorySPtr& intermediate,
                                const dom::CxxProgramSPtr& program,
                                const doim::CxxObjectFileSetSPtr& objectFiles) const;

private:
    doim::SysExecutableSPtr mCompiler;
};
}
