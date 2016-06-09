//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "logex/log.h"
#include <functional>

namespace logex
{
DbsLoggerSPtr gLogger = im::InitializationManager::subscribe(gLogger);
}
