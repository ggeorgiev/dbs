//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/location.hpp"
#include "doim/generic/object.h"
#include "err/gtest/err_assert.h"
#include "gtest/framework.h"
#include <memory>
#include <stdexcept>
#include <str>

TEST(ManagerTest, obtainObject)
{
    const auto& obj =
        doim::Object::obtain(nullptr, doim::Object::EType::kCxxLibrary, "/foo/obj");
    ASSERT_EQ(doim::Object::EType::kCxxLibrary, obj->type());
    ASSERT_STREQ("obj", obj->name().c_str());
}

TEST(ManagerTest, obtainEmptyObject)
{
    ASSERT_EQ(nullptr,
              doim::Object::obtain(nullptr, doim::Object::EType::kCxxLibrary, ""));
    ASSERT_EQ(nullptr,
              doim::Object::obtain(nullptr, doim::Object::EType::kCxxLibrary, "obj"));

    const auto& root = doim::Location::obtain(nullptr, "/");
    ASSERT_EQ(nullptr, doim::Object::obtain(root, doim::Object::EType::kCxxLibrary, "/"));
    ASSERT_EQ(nullptr,
              doim::Object::obtain(root, doim::Object::EType::kCxxLibrary, "foo/"));

    const auto& rootFoo1 =
        doim::Object::obtain(nullptr, doim::Object::EType::kCxxLibrary, "/obj");
    ASSERT_NE(nullptr, rootFoo1);

    const auto& rootFoo2 = doim::Object::obtain(rootFoo1->location(),
                                                doim::Object::EType::kCxxLibrary,
                                                "obj");
    ASSERT_NE(nullptr, rootFoo2);
}

TEST(ManagerTest, obtainUniqueObject)
{
    const auto& root = doim::Location::obtain(nullptr, "/");

    const auto& rootFoo1 =
        doim::Object::obtain(root, doim::Object::EType::kCxxLibrary, "obj");
    const auto& rootFoo2 =
        doim::Object::obtain(nullptr, doim::Object::EType::kCxxLibrary, "/obj");
    ASSERT_EQ(rootFoo1, rootFoo2);
}
