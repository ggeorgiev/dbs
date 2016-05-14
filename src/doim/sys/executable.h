//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_binary.h"
#include "doim/fs/fs_file.h"
#include <iosfwd>
#include <memory>

namespace doim
{
class SysExecutable;
typedef shared_ptr<SysExecutable> SysExecutableSPtr;

class SysExecutable : public Base<SysExecutable, doim::FsFileSPtr, doim::FsBinarySPtr>
{
public:
    SysExecutable(const doim::FsFileSPtr& file);
    SysExecutable(const doim::FsBinarySPtr& binary);

    std::string path() const;
};
}
