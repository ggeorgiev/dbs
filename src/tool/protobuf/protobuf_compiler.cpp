//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/protobuf/protobuf_compiler.h"
#include "doim/cxx/cxx_file.h"
#include "doim/sys/sys_command.h"

namespace tool
{
ProtobufCompiler::ProtobufCompiler(const doim::SysExecutableSPtr& compiler)
    : mCompiler(compiler)
{
}

doim::SysCommandSPtr ProtobufCompiler::compileCommand(
    const doim::FsDirectorySPtr& directory,
    const doim::FsDirectorySPtr& destination,
    const doim::ProtobufFileSPtr& protobufFile,
    const doim::CxxFileSPtr& cxxFile) const
{
    ASSERT(protobufFile != nullptr);
    ASSERT(cxxFile != nullptr);

    auto compileArguments = doim::SysArgumentSet::make();

    auto argument_cpp_out = doim::SysArgument::unique("--cpp_out=" + destination->path());
    compileArguments->insert(argument_cpp_out);

    auto argument_proto =
        doim::SysArgument::unique(protobufFile->file()->path(directory));
    compileArguments->insert(argument_proto);
    compileArguments = doim::SysArgumentSet::unique(compileArguments);

    return doim::SysCommand::unique(directory, mCompiler, compileArguments);
}
}
