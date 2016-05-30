//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/protobuf/protobuf_compiler.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/fs/fs_file.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_command.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include <memory>
#include <str>

namespace tool
{
ProtobufCompiler::ProtobufCompiler(const doim::SysExecutableSPtr& compiler)
    : mCompiler(compiler)
{
}

doim::SysCommandSPtr ProtobufCompiler::compileCommand(
    const doim::FsDirectorySPtr& directory, const doim::CxxFileSPtr& cxxFile) const
{
    return doim::SysCommandSPtr();
}
}
