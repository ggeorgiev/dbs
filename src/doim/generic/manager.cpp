//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/generic/manager.h"

#include "const/constants.h"

#include <__hash_table>
#include <functional>
#include <utility>

namespace doim
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

ObjectSPtr Manager::obtainObject(const LocationSPtr& base,
                                 const Object::Type type,
                                 const std::string::const_iterator& begin,
                                 const std::string::const_iterator& end)
{
    auto pos = end;
    while (--pos >= begin)
    {
        if (*pos == slash())
            break;
    }
    ++pos;

    if (pos == end)
        return ObjectSPtr();

    auto location = obtainLocation(base, begin, pos);
    if (location == nullptr)
        return ObjectSPtr();

    auto working = std::make_shared<Object>();
    working->set_type(type);
    working->set_name(std::string(pos, end));
    working->set_location(location);

    return *mObjects.emplace(working).first;
}
} // namespace doim
