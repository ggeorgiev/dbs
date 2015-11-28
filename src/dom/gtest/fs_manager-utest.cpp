//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/fs/fs_directory.hpp"
#include "dom/fs/fs_manager.h"

#include "gtest/err_assert.h"

#include <gtest/gtest-message.h>
#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <ostream>
#include <string>

TEST(FsManagerTest, obtainEmpty)
{
    auto emptyDir = dom::gFsManager->obtainDirectory(nullptr, "");
    ASSERT_EQ(nullptr, emptyDir);

    auto rootDir = dom::gFsManager->obtainDirectory(nullptr, "/");
    ASSERT_NE(nullptr, rootDir);
    ASSERT_EQ(nullptr, rootDir->parent());
    ASSERT_TRUE(rootDir->name().empty());

    auto dir = dom::gFsManager->obtainDirectory(rootDir, "");
    ASSERT_EQ(dir, rootDir);

    EXPECT_ASSERT(dom::gFsManager->obtainDirectory(nullptr, "bar/"));
    EXPECT_ASSERT(dom::gFsManager->obtainDirectory(rootDir, "/bar/"));
}

TEST(FsManagerTest, obtainUnique)
{
    auto root1 = dom::gFsManager->obtainDirectory(nullptr, "/");
    auto root2 = dom::gFsManager->obtainDirectory(nullptr, "/");
    ASSERT_EQ(root1, root2);

    auto foo1 = dom::gFsManager->obtainDirectory(nullptr, "/foo/");
    auto foo2 = dom::gFsManager->obtainDirectory(nullptr, "/foo/");
    ASSERT_EQ(foo1, foo2);

    auto foo3 = dom::gFsManager->obtainDirectory(root1, "foo/");
    ASSERT_EQ(foo1, foo3);

    auto foo4 = dom::gFsManager->obtainDirectory(foo1, "");
    ASSERT_EQ(foo1, foo4);

    auto foo5 = dom::gFsManager->obtainDirectory(nullptr, "/foo/../foo");
    ASSERT_EQ(foo1, foo5);

    auto bar1 = dom::gFsManager->obtainDirectory(root1, "bar/");
    auto bar2 = dom::gFsManager->obtainDirectory(foo1, "bar/");
    ASSERT_NE(bar1, bar2);

    auto bar3 = dom::gFsManager->obtainDirectory(foo1, "../foo/bar/");
    ASSERT_NE(bar1, bar3);
}

TEST(FsManagerTest, obtain)
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
        Test{.root = "/foo/bar", .dir = "../../foo", .absolute = "/foo/", .relative = "../"},
    };

    for (auto test : tests)
    {
        dom::FsDirectorySPtr root = dom::gFsManager->obtainDirectory(nullptr, test.root);
        dom::FsDirectorySPtr directory = dom::gFsManager->obtainDirectory(root, test.dir);
        ASSERT_NE(nullptr, directory);

        ASSERT_EQ(test.absolute, directory->path(nullptr)) << "root: \"" << test.root
                                                           << "\", dir: \"" << test.dir << "\"";
        ASSERT_EQ(test.relative, directory->path(root)) << "root: \"" << test.root << "\", dir: \""
                                                        << test.dir << "\"";
    }
}
