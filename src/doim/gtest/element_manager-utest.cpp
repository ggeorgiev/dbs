//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/generic/location.hpp"
#include "gtest/framework.h"
#include <memory>
#include <ostream>

TEST(ElementManagerTest, unique)
{
    doim::LocationSPtr location;
    ASSERT_EQ(nullptr, doim::Location::unique(location));

    location = doim::Location::obtain(nullptr, "/unique");
    ASSERT_EQ(location, doim::Location::unique(location));
}

TEST(ElementManagerTest, isUnique)
{
    doim::LocationSPtr location;
    ASSERT_TRUE(location->isUnique());

    location = doim::Location::obtain(nullptr, "/isUnique");
    ASSERT_TRUE(location->isUnique());

    location = doim::Location::make(nullptr, "isUnique");
    ASSERT_FALSE(location->isUnique());
}

TEST(ElementManagerTest, find)
{
    doim::LocationSPtr location;
    ASSERT_EQ(nullptr, location->find());

    location = doim::Location::obtain(nullptr, "/isUnique");
    ASSERT_EQ(location, location->find());

    doim::LocationSPtr location1 = doim::Location::make(location->parent(), "isUnique");
    ASSERT_EQ(location, location1->find());
}
