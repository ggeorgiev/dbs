//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "option/verbose.h"
#include "gtest/framework.h"

TEST(VerboseTest, match)
{
    doim::TagSetSPtr pattern = doim::TagSet::unique(doim::TagSet::make(doim::TagSet{}));
    doim::TagSetSPtr pattern1 =
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gCrcTag}));
    doim::TagSetSPtr pattern2 = doim::TagSet::unique(
        doim::TagSet::make(doim::TagSet{doim::gCrcTag, doim::gDbTag}));

    doim::TagSetSPtr tags = pattern;
    doim::TagSetSPtr tags1 = pattern1;

    ASSERT_TRUE(opt::Verbose::match(pattern, tags));
    ASSERT_TRUE(opt::Verbose::match(pattern, tags1));

    ASSERT_FALSE(opt::Verbose::match(pattern1, tags));
    ASSERT_TRUE(opt::Verbose::match(pattern1, tags1));

    ASSERT_FALSE(opt::Verbose::match(pattern2, tags));
    ASSERT_FALSE(opt::Verbose::match(pattern2, tags1));

    unordered_set<doim::TagSetSPtr> sets;
    sets.insert(pattern1);
    sets.insert(pattern2);

    ASSERT_FALSE(opt::Verbose::match(sets, tags));
    ASSERT_TRUE(opt::Verbose::match(sets, tags1));
}
