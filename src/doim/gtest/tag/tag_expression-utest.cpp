//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag_expression.h"
#include "doim/tag/tag.h"
#include "gtest/framework.h"
#include <iosfwd>

TEST(TagExpressionTest, patternMatch)
{
    doim::TagSetSPtr pattern = doim::TagSet::unique(doim::TagSet::make(doim::TagSet{}));

    doim::TagSetSPtr pattern1 =
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gCrcTag}));
    doim::TagSetSPtr pattern2 = doim::TagSet::unique(
        doim::TagSet::make(doim::TagSet{doim::gCrcTag, doim::gDbTag}));

    doim::TagSetSPtr tags = pattern;
    doim::TagSetSPtr tags1 = pattern1;

    ASSERT_TRUE(doim::TagExpression::match(pattern, tags));
    ASSERT_TRUE(doim::TagExpression::match(pattern, tags1));

    ASSERT_FALSE(doim::TagExpression::match(pattern1, tags));
    ASSERT_TRUE(doim::TagExpression::match(pattern1, tags1));

    ASSERT_FALSE(doim::TagExpression::match(pattern2, tags));
    ASSERT_FALSE(doim::TagExpression::match(pattern2, tags1));
}
