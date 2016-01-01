//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/manager.h"
#include <functional>

namespace dom
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);
} // namespace dom
