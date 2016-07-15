//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "git/err.h"
#include "git/git.h"
#include <functional>
#include <str>
#include <git2.h> // IWYU pragma: keep

namespace git
{
Object::Object()
{
}

Object::Object(git_object* object)
    : mObject(object)
{
    ASSERT(mObject != nullptr);
}

Object::~Object()
{
    if (mObject != nullptr)
        git_object_free(mObject);
}

git_otype Object::type()
{
    return git_object_type(mObject);
}

} // namespace git
