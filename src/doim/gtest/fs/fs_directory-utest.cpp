//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.hpp"

#include <gtest/gtest.h>

#include <iosfwd>
#include <memory>

TEST(FsDirectoryTest, path)
{
    auto root = std::make_shared<doim::FsDirectory>();
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
    ASSERT_EQ(1u, root->level());

    auto directory = std::make_shared<doim::FsDirectory>(root, "");
    ASSERT_EQ(2u, directory->level());
}