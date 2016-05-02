//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tool/cxx_compiler.h"
#include "tpool/task.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/argument.hpp"
#include "doim/sys/executable.hpp"
#include <memory>

namespace tool
{
class CxxIwyu;
typedef std::shared_ptr<CxxIwyu> CxxIwyuSPtr;

class CxxIwyu
{
public:
    CxxIwyu(const doim::SysExecutableSPtr& tool);

    tpool::TaskSPtr iwyuCommand(const doim::FsDirectorySPtr& directory,
                                const doim::CxxFileSPtr& objectFile) const;

private:
    doim::SysExecutableSPtr mTool;
};
}
