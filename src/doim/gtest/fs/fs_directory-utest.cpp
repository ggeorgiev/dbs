//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/manager.h"
#include "gtest/framework.h"
#include <iosfwd>
#include <memory>

TEST(FsDirectoryTest, path)
{
    auto root = std::make_shared<doim::FsDirectory>();
    root = doim::gManager->unique(root);
    ASSERT_STREQ("/", root->path(nullptr).c_str());

    auto directory = std::make_shared<doim::FsDirectory>(nullptr, "foo");
    ASSERT_STREQ("foo/", directory->path(nullptr).c_str());

    directory = std::make_shared<doim::FsDirectory>(root, "foo");
    ASSERT_STREQ("/foo/", directory->path(nullptr).c_str());
    ASSERT_STREQ("foo/", directory->path(root).c_str());
}

TEST(FsDirectoryTest, level)
{
    auto root = std::make_shared<doim::FsDirectory>();
    root = doim::gManager->unique(root);
    ASSERT_EQ(1u, root->level());

    auto directory = std::make_shared<doim::FsDirectory>(root, "");
    ASSERT_EQ(2u, directory->level());
}

TEST(FsDirectoryTest, commonAncestor)
{
    auto root = std::make_shared<doim::FsDirectory>();
    root = doim::gManager->unique(root);
    ASSERT_EQ(root, root->commonAncestor(root));

    auto foo = std::make_shared<doim::FsDirectory>(root, "foo");
    ASSERT_EQ(root, root->commonAncestor(foo));
    ASSERT_EQ(root, foo->commonAncestor(root));

    auto bar = std::make_shared<doim::FsDirectory>(root, "bar");
    ASSERT_EQ(root, bar->commonAncestor(foo));
}
