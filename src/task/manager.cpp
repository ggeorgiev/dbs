//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/manager.h"

#include <functional>

namespace task
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

} // namespace task
