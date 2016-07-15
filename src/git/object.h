//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/url/url.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include <shared_ptr>
#include <git2.h> // IWYU pragma: keep
#include <stddef.h>

namespace git
{
class Object
{
public:
    Object();
    Object(git_object* object);
    ~Object();

    git_object* git()
    {
        return mObject;
    }

    git_otype type();

private:
    git_object* mObject = nullptr;
};

typedef shared_ptr<Object> ObjectSPtr;
}
