//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/protobuf/protobuf_file.h"
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

    doim::SysCommandSPtr compileCommand(const doim::FsDirectorySPtr& destination,
                                        const doim::ProtobufFileSPtr& protobufFile) const;

private:
    doim::SysExecutableSPtr mCompiler;
};
}
