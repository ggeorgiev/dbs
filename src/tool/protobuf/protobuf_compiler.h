//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_command.h"
#include "doim/sys/sys_executable.h"
#include <memory>

namespace tool
{
class ProtobufCompiler;
typedef shared_ptr<ProtobufCompiler> ProtobufCompilerSPtr;

class ProtobufCompiler : public std::enable_shared_from_this<ProtobufCompiler>
{
public:
    ProtobufCompiler(const doim::SysExecutableSPtr& compiler);

    doim::SysCommandSPtr compileCommand(const doim::FsDirectorySPtr& directory,
                                        const doim::CxxFileSPtr& cxxFile) const;

private:
    doim::SysExecutableSPtr mCompiler;
};
}
