//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "doim/generic/location.hpp"
#include "gtest/framework.h"
#include <memory>
#include <ostream>
#include <unordered_set>

TEST(ManagerObjectSetMixinTest, unique)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, doim::gManager->unique(locations));

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/unique");
    locations->insert(location);

    ASSERT_EQ(locations, doim::gManager->unique(locations));
}

TEST(ManagerObjectSetMixinTest, isUnique)
{
    doim::LocationSetSPtr locations;
    ASSERT_TRUE(doim::gManager->isUnique(locations));

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/isUnique");
    locations->insert(location);

    ASSERT_FALSE(doim::gManager->isUnique(locations));

    locations = doim::gManager->unique(locations);
    ASSERT_TRUE(doim::gManager->isUnique(location));
}

TEST(ManagerObjectSetMixinTest, find)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, doim::gManager->find(locations));

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/find");
    locations->insert(location);

    ASSERT_EQ(nullptr, doim::gManager->find(locations));

    locations = doim::gManager->unique(locations);
    ASSERT_EQ(locations, doim::gManager->find(locations));

    auto locations1 = doim::LocationSet::make();
    locations1->insert(location);

    ASSERT_EQ(locations, doim::gManager->find(locations1));
}
