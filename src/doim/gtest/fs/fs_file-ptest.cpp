//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_file.h"
#include "doim/fs/fs_directory.h"
#include "gtest/framework.h"
#include <shared_ptr>

TEST(FsFileTest, VERYSLOW_construct)
{
    doim::FsDirectorySPtr directory;

    for (int i = 0; i < 10000; ++i)
        doim::FsFile::make(directory, "this is a file name");
}

TEST(FsFileTest, obtainEmpty)
{
    ASSERT_EQ(nullptr, doim::FsFile::obtain(nullptr, ""));
    ASSERT_EQ(nullptr, doim::FsFile::obtain(nullptr, "foo.cpp"));

    const auto& root = doim::FsDirectory::obtain(nullptr, "/");
    ASSERT_EQ(nullptr, doim::FsFile::obtain(root, ""));
    ASSERT_EQ(nullptr, doim::FsFile::obtain(root, "/"));
    ASSERT_EQ(nullptr, doim::FsFile::obtain(root, "foo/"));

    const auto& rootFoo1 = doim::FsFile::obtain(nullptr, "/foo.cpp");
    ASSERT_NE(nullptr, rootFoo1);

    const auto& rootFoo2 = doim::FsFile::obtain(rootFoo1->directory(), "foo.cpp");
    ASSERT_NE(nullptr, rootFoo2);
}

TEST(FsFileTest, obtainUnique)
{
    const auto& root = doim::FsDirectory::obtain(nullptr, "/");

    const auto& rootFoo1 = doim::FsFile::obtain(root, "foo.cpp");
    const auto& rootFoo2 = doim::FsFile::obtain(nullptr, "/foo.cpp");
    ASSERT_EQ(rootFoo1, rootFoo2);
}
