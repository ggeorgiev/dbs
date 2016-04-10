//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/tpool.h"

#include <functional>

namespace task
{
TPoolSPtr gTPool = im::InitializationManager::subscribe(gTPool);

} // namespace task
