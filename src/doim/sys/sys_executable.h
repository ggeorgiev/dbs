//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/fs/fs_binary.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <str>

namespace doim
{
class SysExecutable;
typedef shared_ptr<SysExecutable> SysExecutableSPtr;

class SysExecutable : public Element<SysExecutable, doim::FsFileSPtr, doim::FsBinarySPtr>
{
public:
    SysExecutable(const doim::FsFileSPtr& file, const doim::FsBinarySPtr& binary);

    string path() const;
};
}
