//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/command.h"
#include "doim/sys/executable.h"
#include <memory>

namespace tool
{
class CxxCompiler;
typedef shared_ptr<CxxCompiler> CxxCompilerSPtr;

class CxxCompiler : public std::enable_shared_from_this<CxxCompiler>
{
public:
    static doim::SysArgumentSetSPtr includeArguments(
        const doim::FsDirectorySPtr& directory,
        const doim::CxxIncludeDirectorySetSPtr& includeDirectories);

    CxxCompiler(const doim::SysExecutableSPtr& compiler);

    doim::SysCommandSPtr compileCommand(const doim::FsDirectorySPtr& directory,
                                        const doim::CxxObjectFileSPtr& objectFile) const;

    doim::SysCommandSPtr linkCommand(const doim::FsDirectorySPtr& directory,
                                     const doim::CxxProgramSPtr& program) const;

private:
    static doim::SysArgumentSPtr gOptimizationLevel0Argument;
    static doim::SysArgumentSPtr gOptimizationLevel3Argument;
    static doim::SysArgumentSPtr gDebuggingInformationArgument;
    static doim::SysArgumentSPtr gStdCpp11Argument;
    static doim::SysArgumentSPtr gFProfileArcArgument;
    static doim::SysArgumentSPtr gFTestCoverageArgument;

    static doim::SysArgumentSPtr gCoverageArgument;
    static doim::SysArgumentSPtr gStdLibc11Argument;

    doim::SysExecutableSPtr mCompiler;
};
}
