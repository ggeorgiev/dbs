//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/generic/location.hpp"
#include "doim/generic/object.h"
#include <gtest/gtest.h>
#include <memory>
#include <str>

TEST(ManagerTest, obtainObject)
{
    const auto& obj =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, nullptr, "/foo/obj");
    ASSERT_EQ(doim::Object::EType::kCxxLibrary, obj->type());
    ASSERT_STREQ("obj", obj->name().c_str());
}

TEST(ManagerTest, obtainEmptyObject)
{
    ASSERT_EQ(nullptr,
              doim::Object::obtain(doim::Object::EType::kCxxLibrary, nullptr, ""));
    ASSERT_EQ(nullptr,
              doim::Object::obtain(doim::Object::EType::kCxxLibrary, nullptr, "obj"));

    const auto& root = doim::Location::obtain(nullptr, "/");
    ASSERT_EQ(nullptr, doim::Object::obtain(doim::Object::EType::kCxxLibrary, root, "/"));
    ASSERT_EQ(nullptr,
              doim::Object::obtain(doim::Object::EType::kCxxLibrary, root, "foo/"));

    const auto& rootFoo1 =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, nullptr, "/obj");
    ASSERT_NE(nullptr, rootFoo1);

    const auto& rootFoo2 = doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                                                rootFoo1->location(),
                                                "obj");
    ASSERT_NE(nullptr, rootFoo2);
}

TEST(ManagerTest, obtainUniqueObject)
{
    const auto& root = doim::Location::obtain(nullptr, "/");

    const auto& rootFoo1 =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, root, "obj");
    const auto& rootFoo2 =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, nullptr, "/obj");
    ASSERT_EQ(rootFoo1, rootFoo2);
}
