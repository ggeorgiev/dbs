//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/executable.h"
#include <tuple>

namespace doim
{
SysExecutable::SysExecutable(const doim::FsFileSPtr& file)
    : Base(file, nullptr)
{
}
SysExecutable::SysExecutable(const doim::FsBinarySPtr& binary)
    : Base(nullptr, binary)
{
}

std::string SysExecutable::path() const
{
    const doim::FsFileSPtr& file = std::get<0>(mArgs);
    if (file != nullptr)
        return file->path();

    const doim::FsBinarySPtr& binary = std::get<1>(mArgs);
    return binary->name();
}
}
