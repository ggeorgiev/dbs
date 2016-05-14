//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/manager.h"
#include "gtest/framework.h"
#include <memory>
#include <str>

TEST(FsDirectoryTest, path)
{
    auto root = doim::unique<doim::FsDirectory>();
    ASSERT_STREQ("/", root->path(nullptr).c_str());

    auto directory = doim::FsDirectory::make(nullptr, "foo");
    ASSERT_STREQ("foo/", directory->path(nullptr).c_str());

    directory = doim::FsDirectory::make(root, "foo");
    ASSERT_STREQ("/foo/", directory->path(nullptr).c_str());
    ASSERT_STREQ("foo/", directory->path(root).c_str());
}

TEST(FsDirectoryTest, level)
{
    auto root = doim::unique<doim::FsDirectory>();
    ASSERT_EQ(1u, root->level());

    auto directory = doim::FsDirectory::make(root, "");
    ASSERT_EQ(2u, directory->level());
}

TEST(FsDirectoryTest, commonAncestor)
{
    auto root = doim::unique<doim::FsDirectory>();
    ASSERT_EQ(root, root->commonAncestor(root));

    auto foo = doim::FsDirectory::make(root, "foo");
    ASSERT_EQ(root, root->commonAncestor(foo));
    ASSERT_EQ(root, foo->commonAncestor(root));

    auto bar = doim::FsDirectory::make(root, "bar");
    ASSERT_EQ(root, bar->commonAncestor(foo));
}
