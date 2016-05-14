//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/executable.h"
#include <memory>

namespace tool
{
class CxxIwyu;
typedef shared_ptr<CxxIwyu> CxxIwyuSPtr;

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
