//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "err/err.h"

namespace err
{
thread_local ErrorUPtr gError;

static bool init()
{
    gNames[kSuccess] = "success";

    gNames[kExpected] = "expected";
    gNames[kAssert] = "assert";

    gNames[kNotFound] = "not found";
    gNames[kTooMany] = "too many";
    gNames[kUnable] = "unable";

    return true;
}

NameMap gNames =
    im::InitializationManager::subscribe<NameMap>(initialization_rank(), &init, nullptr);

} // namespace err
