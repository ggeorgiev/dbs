//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "doim/generic/location.hpp"
#include "gtest/framework.h"
#include <memory>
#include <ostream>
#include <unordered>

TEST(ManagerObjectSetMixinTest, unique)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, doim::LocationSet::unique(locations));

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/unique");
    locations->insert(location);

    ASSERT_EQ(locations, doim::LocationSet::unique(locations));
}

TEST(ManagerObjectSetMixinTest, isUnique)
{
    doim::LocationSetSPtr locations;
    ASSERT_TRUE(locations->isUnique());

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/isUnique");
    locations->insert(location);

    ASSERT_FALSE(locations->isUnique());

    locations = doim::LocationSet::unique(locations);
    ASSERT_TRUE(location->isUnique());
}

TEST(ManagerObjectSetMixinTest, find)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, locations->find());

    locations = doim::LocationSet::make();

    auto location = doim::gManager->obtainLocation(nullptr, "/find");
    locations->insert(location);

    ASSERT_EQ(nullptr, locations->find());

    locations = doim::LocationSet::unique(locations);
    ASSERT_EQ(locations, locations->find());

    auto locations1 = doim::LocationSet::make();
    locations1->insert(location);

    ASSERT_EQ(locations, locations1->find());
}
