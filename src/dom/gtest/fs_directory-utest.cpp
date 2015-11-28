//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/fs/fs_directory.hpp"

#include <gtest/gtest.h>

#include <iosfwd>
#include <memory>

TEST(FsDirectoryTest, path)
{
    dom::FsDirectorySPtr root = std::make_shared<dom::FsDirectory>();
    ASSERT_STREQ("/", root->path(nullptr).c_str());

    dom::FsDirectorySPtr directory = std::make_shared<dom::FsDirectory>();
    directory->set_name("foo");
    ASSERT_STREQ("foo/", directory->path(nullptr).c_str());

    directory->set_parent(root);
    ASSERT_STREQ("/foo/", directory->path(nullptr).c_str());

    directory->set_parent(root);
    ASSERT_STREQ("foo/", directory->path(root).c_str());
}

TEST(FsDirectoryTest, level)
{
    dom::FsDirectorySPtr root = std::make_shared<dom::FsDirectory>();
    ASSERT_EQ(1u, root->level());

    dom::FsDirectorySPtr directory = std::make_shared<dom::FsDirectory>();
    directory->set_parent(root);
    ASSERT_EQ(2u, directory->level());
}
