//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/sys_executable.h"
#include "err/err_assert.h"
#include <tuple>

namespace doim
{
SysExecutable::SysExecutable(const doim::FsFileSPtr& file,
                             const doim::FsBinarySPtr& binary)
    : Element(file, binary)
{
    ASSERT(file == nullptr || binary == nullptr);
    ASSERT(file != nullptr || binary != nullptr);
}

string SysExecutable::path(const doim::FsDirectorySPtr& root) const
{
    const doim::FsFileSPtr& file = std::get<0>(mArgs);
    if (file != nullptr)
        return file->path(root);

    const doim::FsBinarySPtr& binary = std::get<1>(mArgs);
    return binary->name();
}
}
