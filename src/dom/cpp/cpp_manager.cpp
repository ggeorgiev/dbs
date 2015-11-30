//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cpp/cpp_manager.h"

#include <functional>

namespace dom
{
CppManagerSPtr gCppManager = im::InitializationManager::subscribe(gCppManager);
}
