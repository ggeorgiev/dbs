//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "err/err.h"
#include <functional>

namespace err
{
boost::thread_specific_ptr<Error> gError;

static bool init()
{
    gNames[kSuccess] = "success";

    gNames[kExpected] = "expected";
    gNames[kAssert] = "assert";

    gNames[kFailed] = "failed";
    gNames[kNotFound] = "not found";
    gNames[kTooMany] = "too many";
    gNames[kUnable] = "unable";
    gNames[kUnexpected] = "unexpected";
    gNames[kUnknown] = "unknown";

    gNames[kDatabase] = "database";
    gNames[kGit] = "git";
    gNames[kFileSystem] = "file system";

    return true;
}

NameMap gNames = im::InitializationManager::subscribe<NameMap>(rank(), &init, nullptr);

} // namespace err
