//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_iwyu.h"
#include "task/manager.h"
#include "task/sys/execute_command_task.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.h"
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tool
{
CxxIwyu::CxxIwyu(const doim::SysExecutableSPtr& tool)
    : mTool(tool)
{
}

tpool::TaskSPtr CxxIwyu::iwyuCommand(const doim::FsDirectorySPtr& directory,
                                     const doim::CxxFileSPtr& cxxFile) const
{
    auto arguments =
        CxxCompiler::includeArguments(directory, cxxFile->cxxIncludeDirectories());

    auto argument_cxxflags = doim::gManager->obtainArgument(
        "-std=c++11 -stdlib=libc++ -O0 -g "
        "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/"
        "MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk");
    arguments->insert(argument_cxxflags);

    const std::string& file = cxxFile->file()->path(directory);

    auto argument_file = doim::gManager->obtainArgument(file);
    arguments->insert(argument_file);
    arguments = doim::gManager->unique(arguments);

    auto command = std::make_shared<doim::SysCommand>(mTool, arguments);
    command = doim::gManager->unique(command);
    auto task = task::gManager->valid(
        std::make_shared<task::ExecuteCommandTask>(command,
                                                   task::EExitExpectation::kAny,
                                                   "Iwyu " + file));

    return task;
}
}
