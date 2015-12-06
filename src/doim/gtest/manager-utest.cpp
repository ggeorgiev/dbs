//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/manager.h"

#include "doim/fs/fs_file.hpp"
#include "doim/fs/fs_directory.hpp"

#include "gtest/err_assert.h"

#include <gtest/gtest-message.h>
#include <gtest/gtest.h>

#include <memory>
#include <ostream>
#include <string>

TEST(ManagerTest, obtainEmptyDirectory)
{
    const auto& emptyDir = doim::gManager->obtainDirectory(nullptr, "");
    ASSERT_EQ(nullptr, emptyDir);

    const auto& rootDir = doim::gManager->obtainDirectory(nullptr, "/");
    ASSERT_NE(nullptr, rootDir);
    ASSERT_EQ(nullptr, rootDir->parent());
    ASSERT_TRUE(rootDir->name().empty());

    const auto& dir = doim::gManager->obtainDirectory(rootDir, "");
    ASSERT_EQ(dir, rootDir);

    ASSERT_ASSERT(doim::gManager->obtainDirectory(nullptr, "bar/"));
    ASSERT_ASSERT(doim::gManager->obtainDirectory(rootDir, "/bar/"));
}

TEST(ManagerTest, obtainUniqueDirectory)
{
    const auto& root1 = doim::gManager->obtainDirectory(nullptr, "/");
    const auto& root2 = doim::gManager->obtainDirectory(nullptr, "/");
    ASSERT_EQ(root1, root2);

    const auto& foo1 = doim::gManager->obtainDirectory(nullptr, "/foo/");
    const auto& foo2 = doim::gManager->obtainDirectory(nullptr, "/foo/");
    ASSERT_EQ(foo1, foo2);

    const auto& foo3 = doim::gManager->obtainDirectory(root1, "foo/");
    ASSERT_EQ(foo1, foo3);

    const auto& foo4 = doim::gManager->obtainDirectory(foo1, "");
    ASSERT_EQ(foo1, foo4);

    const auto& foo5 = doim::gManager->obtainDirectory(nullptr, "/foo/../foo");
    ASSERT_EQ(foo1, foo5);

    const auto& bar1 = doim::gManager->obtainDirectory(root1, "bar/");
    const auto& bar2 = doim::gManager->obtainDirectory(foo1, "bar/");
    ASSERT_NE(bar1, bar2);

    const auto& bar3 = doim::gManager->obtainDirectory(foo1, "../foo/bar/");
    ASSERT_NE(bar1, bar3);
}

TEST(ManagerTest, obtainDirectory)
{
    struct Test
    {
        std::string root;
        std::string dir;
        std::string absolute;
        std::string relative;
    };

    Test tests[]{
        Test{.root = "", .dir = "/", .absolute = "/", .relative = "/"},
        Test{.root = "/", .dir = "", .absolute = "/", .relative = ""},
        Test{.root = "", .dir = "////", .absolute = "/", .relative = "/"},
        Test{.root = "", .dir = "/foo", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "", .dir = "/foo/", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "", .dir = "///foo///", .absolute = "/foo/", .relative = "/foo/"},
        Test{.root = "/foo/", .dir = "bar", .absolute = "/foo/bar/", .relative = "bar/"},
        Test{.root = "/foo/", .dir = "..", .absolute = "/", .relative = "../"},
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
        const auto& root = doim::gManager->obtainDirectory(nullptr, test.root);
        const auto& directory = doim::gManager->obtainDirectory(root, test.dir);
        ASSERT_NE(nullptr, directory);

        ASSERT_EQ(test.absolute, directory->path(nullptr))
            << "root: \"" << test.root << "\", dir: \"" << test.dir << "\"";
        ASSERT_EQ(test.relative, directory->path(root))
            << "root: \"" << test.root << "\", dir: \"" << test.dir << "\"";
    }
}

TEST(ManagerTest, obtainEmptyFile)
{
    ASSERT_EQ(nullptr, doim::gManager->obtainFile(nullptr, ""));
    ASSERT_EQ(nullptr, doim::gManager->obtainFile(nullptr, "foo.cpp"));

    const auto& root = doim::gManager->obtainDirectory(nullptr, "/");
    ASSERT_EQ(nullptr, doim::gManager->obtainFile(root, "/"));
    ASSERT_EQ(nullptr, doim::gManager->obtainFile(root, "foo/"));

    const auto& rootFoo1 = doim::gManager->obtainFile(nullptr, "/foo.cpp");
    ASSERT_NE(nullptr, rootFoo1);

    const auto& rootFoo2 = doim::gManager->obtainFile(rootFoo1->directory(), "foo.cpp");
    ASSERT_NE(nullptr, rootFoo2);
}

TEST(ManagerTest, obtainUniqueFile)
{
    const auto& root = doim::gManager->obtainDirectory(nullptr, "/");

    const auto& rootFoo1 = doim::gManager->obtainFile(root, "foo.cpp");
    const auto& rootFoo2 = doim::gManager->obtainFile(nullptr, "/foo.cpp");
    ASSERT_EQ(rootFoo1, rootFoo2);
}
