//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "err/err_assert.h"
#include "const/constants.h"
#include <algorithm>
#include <functional>
#include <shared_ptr>
#include <str>

namespace doim
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

} // namespace doim
