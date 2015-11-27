//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_manager.h"

FsManagerSPtr gFsManager = InitializationManager::subscribe(gFsManager);
