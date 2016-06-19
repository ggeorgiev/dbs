//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/git/git2.h"
#include "task/tpool.h"
#include "log/log.h"
#include <git2.h>

namespace task
{
Git2SPtr gGit = im::InitializationManager::subscribe(gGit);

Git2::Git2()
{
    // git_libgit2_init();
}

Git2::~Git2()
{
    // git_libgit2_shutdown();
}

doim::UrlSPtr Git2::url(const doim::FsDirectorySPtr& repoDir)
{
    return nullptr;
}

} // namespace task
