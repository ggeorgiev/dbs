//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/generic/object.h"
#include "doim/fs/fs_directory.h"
#include "err/err_assert.h"
#include "const/constants.h"

namespace doim
{
Object::Object(const EType type, const LocationSPtr& location, const string& name)
    : Element(type, location, name)
{
    ASSERT(location->isUnique());
}

ObjectSPtr Object::obtain(const Object::EType type,
                          const LocationSPtr& base,
                          const string_view& object)
{
    auto pos = object.size();
    while (pos-- > 0)
    {
        if (object[pos] == slash())
            break;
    }
    ++pos;

    if (pos == object.size())
        return ObjectSPtr();

    auto location = Location::obtain(base, string_view(object.begin(), pos));
    if (location == nullptr)
        return ObjectSPtr();

    return unique(type, location, string(object.begin() + pos, object.end()));
}
}
