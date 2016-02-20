//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/generic/object.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
Object::Object(const Type type, const std::string& name, const LocationSPtr& location)
    : Base(static_cast<int>(type), name, location)
{
    ASSERT(gManager->isUnique(location));
}
}
