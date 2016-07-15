//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/url/url.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest.h>
#include <boost/filesystem/operations.hpp>
#include "git/git.h"
#include <memory>
#include <str>

TEST(RepoTest, INTEGRATION_parseRevision)
{
    auto repoDir = doim::FsDirectory::obtain(testing::gTempDirectory, "parse-revision");
    boost::filesystem::remove_all(repoDir->path().c_str());
    auto url = doim::Url::make("https://github.com/ggeorgiev/dev-scripts.git");

    git::RepoSPtr repo;
    ASSERT_OKAY(git::gMgr->cloneRepo(url, repoDir, repo));
    ASSERT_NE(nullptr, repo);

    git::ObjectSPtr object;
    ASSERT_BANNED(kGit,
                  repo->parseRevision("0000000000000000000000000000000000000000",
                                      object));

    ASSERT_OKAY(repo->parseRevision("305402f92a5078053a8c13db9dacb679d801bed3", object));
    EXPECT_NE(object->git(), nullptr);
    ASSERT_EQ(object->type(), GIT_OBJ_COMMIT);
}

TEST(RepoTest, INTEGRATION_checkout)
{
    auto repoDir = doim::FsDirectory::obtain(testing::gTempDirectory, "repo_checkout");
    boost::filesystem::remove_all(repoDir->path().c_str());
    auto url = doim::Url::make("https://github.com/ggeorgiev/dev-scripts.git");

    git::RepoSPtr repo;
    ASSERT_OKAY(git::gMgr->cloneRepo(url, repoDir, repo));
    ASSERT_NE(nullptr, repo);

    git::ObjectSPtr object;
    ASSERT_OKAY(repo->parseRevision("305402f92a5078053a8c13db9dacb679d801bed3", object));
    ASSERT_OKAY(repo->checkout(object));
}
