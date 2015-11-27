//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"
#include "dom/fs/fs_file.hpp"

#include <memory>

namespace dom
{
class FsManager
{
public:

};

typedef std::shared_ptr<FsManager> FsManagerSPtr;

extern FsManagerSPtr gFsManager;

}
