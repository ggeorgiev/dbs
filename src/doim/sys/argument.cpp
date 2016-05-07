//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/argument.h"
#include "doim/manager.h"
#include <functional>

namespace doim
{
SysArgumentSPtr SysArgument::global(const std::string& value, SysArgumentSPtr& argument)
{
    return Manager::global<SysArgument, std::string>(value, argument);
}
}
