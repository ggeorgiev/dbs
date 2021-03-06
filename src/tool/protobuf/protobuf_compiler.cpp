//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/protobuf/protobuf_compiler.h"
#include "doim/fs/fs_file.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_command.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include <string>

namespace tool
{
ProtobufCompiler::ProtobufCompiler(const doim::SysExecutableSPtr& compiler)
    : mCompiler(compiler)
{
}

doim::SysCommandSPtr ProtobufCompiler::compileCommand(
    const doim::FsDirectorySPtr& destination,
    const doim::ProtobufFileSPtr& protobufFile) const
{
    ASSERT(protobufFile != nullptr);

    auto compileArguments = doim::SysArgumentSet::make();

    auto argument_cpp_out = doim::SysArgument::unique(
        "--cpp_out=" + protobufFile->directory()->nonEmptyPath(destination));

    compileArguments->insert(argument_cpp_out);

    auto argument_proto =
        doim::SysArgument::unique(protobufFile->file()->path(protobufFile->directory()));
    compileArguments->insert(argument_proto);
    compileArguments = doim::SysArgumentSet::unique(compileArguments);

    return doim::SysCommand::unique(protobufFile->directory(),
                                    mCompiler,
                                    compileArguments);
}
}
