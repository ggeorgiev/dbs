//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/set.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/generic/location.hpp"
#include "doim/tag/tag.h"
#include <gtest/gtest.h>
#include <memory>
#include <ostream>

TEST(SetManagerTest, unique)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, doim::LocationSet::unique(locations));

    locations = doim::LocationSet::make();

    auto location = doim::Location::obtain(nullptr, "/unique");
    locations->insert(location);

    ASSERT_EQ(locations, doim::LocationSet::unique(locations));
}

TEST(SetManagerTest, isUnique)
{
    doim::LocationSetSPtr locations;
    ASSERT_TRUE(locations->isUnique());

    locations = doim::LocationSet::make();

    auto location = doim::Location::obtain(nullptr, "/isUnique");
    locations->insert(location);

    ASSERT_FALSE(locations->isUnique());

    locations = doim::LocationSet::unique(locations);
    ASSERT_TRUE(location->isUnique());
}

TEST(SetManagerTest, find)
{
    doim::LocationSetSPtr locations;
    ASSERT_EQ(nullptr, locations->find());

    locations = doim::LocationSet::make();

    auto location = doim::Location::obtain(nullptr, "/find");
    locations->insert(location);

    ASSERT_EQ(nullptr, locations->find());

    locations = doim::LocationSet::unique(locations);
    ASSERT_EQ(locations, locations->find());

    auto locations1 = doim::LocationSet::make();
    locations1->insert(location);

    ASSERT_EQ(locations, locations1->find());
}

TEST(SetManagerTest, combine)
{
    auto tags1 = doim::TagSet::unique({doim::gTaskTag, doim::gRunTag, doim::gDoneTag});
    auto tags2 = doim::TagSet::unique({doim::gCrcTag, doim::gRunTag, doim::gDoneTag});

    auto comb1 = tags1->combine(tags2);
    auto comb2 = tags2->combine(tags1);

    EXPECT_EQ(comb1, comb2);
    EXPECT_EQ(4, comb1->size());

    EXPECT_TRUE(comb1->count(doim::gTaskTag) > 0);
    EXPECT_TRUE(comb1->count(doim::gRunTag) > 0);
    EXPECT_TRUE(comb1->count(doim::gDoneTag) > 0);
    EXPECT_TRUE(comb1->count(doim::gCrcTag) > 0);
}