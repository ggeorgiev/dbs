//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "doim/fs/fs_directory.h"
#include "doim/generic/location.hpp"
#include "gtest/framework.h"
#include <memory>
#include <ostream>

TEST(ManagerObjectMixinTest, unique)
{
    doim::LocationSPtr location;
    ASSERT_EQ(nullptr, doim::gManager->unique(location));

    location = doim::gManager->obtainLocation(nullptr, "/unique");
    ASSERT_EQ(location, doim::gManager->unique(location));
}

TEST(ManagerObjectMixinTest, isUnique)
{
    doim::LocationSPtr location;
    ASSERT_TRUE(doim::gManager->isUnique(location));

    location = doim::gManager->obtainLocation(nullptr, "/isUnique");
    ASSERT_TRUE(doim::gManager->isUnique(location));

    location = doim::Location::make(nullptr, "isUnique");
    ASSERT_FALSE(doim::gManager->isUnique(location));
}

TEST(ManagerObjectMixinTest, find)
{
    doim::LocationSPtr location;
    ASSERT_EQ(nullptr, doim::gManager->find(location));

    location = doim::gManager->obtainLocation(nullptr, "/isUnique");
    ASSERT_EQ(location, doim::gManager->find(location));

    doim::LocationSPtr location1 = doim::Location::make(location->parent(), "isUnique");
    ASSERT_EQ(location, doim::gManager->find(location1));
}
