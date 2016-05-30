//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_binary.h"
#include "doim/fs/fs_file.h"
#include "doim/element.hpp"
#include <memory>
#include <str>

namespace doim
{
class SysExecutable;
typedef shared_ptr<SysExecutable> SysExecutableSPtr;

// TODO: switch to variant
class SysExecutable : public Element<SysExecutable, doim::FsFileSPtr, doim::FsBinarySPtr>
{
public:
    SysExecutable(const doim::FsFileSPtr& file, const doim::FsBinarySPtr& binary);

    string path(const doim::FsDirectorySPtr& root) const;
};
}
