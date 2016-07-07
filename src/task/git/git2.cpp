//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/git/git2.h"
#include <functional>
#include <str>

#define EHGitTest(expression, ...)      \
    do                                  \
    {                                   \
        int error = (expression);       \
        if (error < 0)                  \
            EHBan(kGit, ##__VA_ARGS__); \
    } while (false)

namespace git
{
MgrSPtr gMgr = im::InitializationManager::subscribe(gMgr);

Mgr::Mgr()
{
    git_libgit2_init();
}

Mgr::~Mgr()
{
    git_libgit2_shutdown();
}

ECode Mgr::initRepo(const doim::FsDirectorySPtr& repoDir, RepoSPtr& repo)
{
    auto result = std::make_shared<Repo>();
    EHGitTest(git_repository_init(&result->repo, repoDir->path().c_str(), false));

    repo = result;
    EHEnd;
}

ECode Mgr::openRepo(const doim::FsDirectorySPtr& repoDir, RepoSPtr& repo)
{
    auto result = std::make_shared<Repo>();
    EHGitTest(git_repository_open(&result->repo, repoDir->path().c_str()));

    repo = result;
    EHEnd;
}

ECode Mgr::cloneRepo(const doim::UrlSPtr& url,
                     const doim::FsDirectorySPtr& repoDir,
                     RepoSPtr& repo)
{
    auto result = std::make_shared<Repo>();
    EHGitTest(
        git_clone(&result->repo, url->path().c_str(), repoDir->path().c_str(), nullptr));

    repo = result;
    EHEnd;
}

} // namespace git
