//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "git/err.h"
#include "git/git.h"
#include <functional>
#include <str>
#include <git2.h> // IWYU pragma: keep

namespace git
{
Repo::Repo(git_repository* repo)
    : mRepo(repo)
{
    ASSERT(mRepo != nullptr);
}

Repo::~Repo()
{
    ASSERT(mRepo != nullptr);
    git_repository_free(mRepo);
}

ECode Repo::parseRevision(const string& revision, ObjectSPtr& object)
{
    git_object* obj = nullptr;
    EHGitTest(git_revparse_single(&obj, mRepo, revision.c_str()));
    object = std::make_shared<Object>(obj);
    EHEnd;
}

ECode Repo::checkout(const git::ObjectSPtr& object)
{
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_FORCE;
    EHGitTest(git_checkout_tree(mRepo, object->git(), &opts));
    EHEnd;
}

} // namespace git
