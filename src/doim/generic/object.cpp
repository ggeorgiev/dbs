//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/generic/object.h"
#include "doim/fs/fs_directory.h"
#include "err/err_assert.h"
#include "const/constants.h"

namespace doim
{
Object::Object(const EType type, const string& name, const LocationSPtr& location)
    : Element(type, name, location)
{
    ASSERT(location->isUnique());
}

ObjectSPtr Object::obtain(const LocationSPtr& base,
                          const Object::EType type,
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

    return unique(type, string(object.begin() + pos, object.end()), location);
}
}
