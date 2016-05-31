//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "err/gtest/err_assert.h"
#include "gtest/framework.h"
#include <memory>
#include <stdexcept>
#include <str>

TEST(FsDirectoryTest, path)
{
    auto root = doim::FsDirectory::obtain(nullptr, "/");
    ASSERT_STREQ("/", root->path(nullptr).c_str());

    auto foo = doim::FsDirectory::obtain(nullptr, "/foo");
    ASSERT_STREQ("/foo/", foo->path(nullptr).c_str());
    ASSERT_STREQ("foo/", foo->path(root).c_str());

    auto rootFoo = doim::FsDirectory::obtain(root, "foo");
    ASSERT_EQ(rootFoo, foo);

    auto directory1 = doim::FsDirectory::obtain(nullptr, "/foo1/bar1");
    auto directory2 = doim::FsDirectory::obtain(nullptr, "/foo2/bar2");

    ASSERT_STREQ("/foo1/bar1/", directory1->path(directory2).c_str());
}

TEST(FsDirectoryTest, level)
{
    auto root = doim::FsDirectory::unique(nullptr, "");
    ASSERT_EQ(1u, root->level());

    auto directory = doim::FsDirectory::make(root, "");
    ASSERT_EQ(2u, directory->level());
}

TEST(FsDirectoryTest, commonAncestor)
{
    auto root = doim::FsDirectory::unique(nullptr, "");
    ASSERT_EQ(root, root->commonAncestor(root));

    auto foo = doim::FsDirectory::make(root, "foo");
    ASSERT_EQ(root, root->commonAncestor(foo));
    ASSERT_EQ(root, foo->commonAncestor(root));

    auto bar = doim::FsDirectory::make(root, "bar");
    ASSERT_EQ(root, bar->commonAncestor(foo));
}

TEST(FsDirectoryTest, obtainEmpty)
{
    const auto& emptyDir = doim::FsDirectory::obtain(nullptr, "");
    ASSERT_EQ(nullptr, emptyDir);

    const auto& rootDir = doim::FsDirectory::obtain(nullptr, "/");
    ASSERT_NE(nullptr, rootDir);
    ASSERT_EQ(nullptr, rootDir->parent());
    ASSERT_TRUE(rootDir->name().empty());

    const auto& dir = doim::FsDirectory::obtain(rootDir, "");
    ASSERT_EQ(dir, rootDir);

    ASSERT_ASSERT(doim::FsDirectory::obtain(nullptr, "bar/"));
}

TEST(FsDirectoryTest, obtainUnique)
{
    const auto& root1 = doim::FsDirectory::obtain(nullptr, "/");
    const auto& root2 = doim::FsDirectory::obtain(nullptr, "/");
    ASSERT_EQ(root1, root2);

    const auto& foo1 = doim::FsDirectory::obtain(nullptr, "/foo/");
    const auto& foo2 = doim::FsDirectory::obtain(nullptr, "/foo/");
    ASSERT_EQ(foo1, foo2);

    const auto& foo3 = doim::FsDirectory::obtain(root1, "foo/");
    ASSERT_EQ(foo1, foo3);

    const auto& foo4 = doim::FsDirectory::obtain(foo1, "");
    ASSERT_EQ(foo1, foo4);

    const auto& foo5 = doim::FsDirectory::obtain(nullptr, "/foo/../foo");
    ASSERT_EQ(foo1, foo5);

    const auto& bar1 = doim::FsDirectory::obtain(root1, "bar/");
    const auto& bar2 = doim::FsDirectory::obtain(foo1, "bar/");
    ASSERT_NE(bar1, bar2);

    const auto& bar3 = doim::FsDirectory::obtain(foo1, "../foo/bar/");
    ASSERT_NE(bar1, bar3);
}

TEST(FsDirectoryTest, obtain)
{
    struct Test
    {
        string root;
        string dir;
        string absolute;
        string relative;
    };

    Test tests[]{
        Test{.root = "", .dir = "/", .absolute = "/", .relative = "/"},
        Test{.root = "/", .dir = "", .absolute = "/", .relative = ""},
        Test{.root = "", .dir = "////", .absolute = "/", .relative = "/"},
        Test{.root = "", .dir = "/foo", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "", .dir = "/foo/", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "/bar", .dir = "/foo", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "", .dir = "///foo///", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "/foo/", .dir = "bar", .absolute = "/foo/bar/", .relative = "bar/"},
        Test{.root = "/foo/", .dir = "..", .absolute = "/", .relative = "/"},
        Test{.root = "/foo/bar", .dir = "../", .absolute = "/foo/", .relative = "../"},
        Test{.root = "/foo/bar",
             .dir = "../../foo",
             .absolute = "/foo/",
             .relative = "../"},

        // TODO: add absolute dirs.
        // TODO: add independent directories.
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE("root:" + test.root);
        SCOPED_TRACE("dir:" + test.dir);
        SCOPED_TRACE("absolute:" + test.absolute);
        SCOPED_TRACE("relative:" + test.relative);

        const auto& root = doim::FsDirectory::obtain(nullptr, test.root);
        const auto& directory = doim::FsDirectory::obtain(root, test.dir);
        ASSERT_NE(nullptr, directory);

        ASSERT_EQ(test.absolute, directory->path()) << "root: \"" << test.root
                                                    << "\", dir: \"" << test.dir << "\"";
        ASSERT_EQ(test.relative, directory->path(root))
            << "root: \"" << test.root << "\", dir: \"" << test.dir << "\"";
    }
}

TEST(FsDirectoryTest, corresponding)
{
    struct Test
    {
        string dir;
        string from;
        string to;
        string corresponding;
    };

    Test tests[]{
        Test{
            .from = "/", .dir = "foo/bar", .to = "/baz", .corresponding = "/baz/foo/bar"},
    };

    for (const auto& test : tests)
    {
        const auto& from = doim::FsDirectory::obtain(nullptr, test.from);
        const auto& directory = doim::FsDirectory::obtain(from, test.dir);
        const auto& to = doim::FsDirectory::obtain(nullptr, test.to);
        const auto& expected = doim::FsDirectory::obtain(nullptr, test.corresponding);
        const auto& actual = doim::FsDirectory::corresponding(directory, from, to);

        ASSERT_EQ(expected->path(nullptr), actual->path(nullptr));
    }
}
