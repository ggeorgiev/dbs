//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"

#include <memory>
#include <string>

namespace dom
{
class FsFile
{
public:

private:
    FsDirectory mDirectory;
    std::string mName;
};

typedef std::shared_ptr<FsFile> FsFileSPtr;
}
