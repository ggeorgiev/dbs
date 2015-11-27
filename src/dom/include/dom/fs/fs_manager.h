//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"
#include "dom/fs/fs_file.hpp"

#include "im/initialization_manager.h"

#include <memory>

namespace dom
{
class FsManager
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() + im::InitializationManager::rank_step();
    }
};
};

typedef std::shared_ptr<FsManager> FsManagerSPtr;

extern FsManagerSPtr gFsManager;
}
