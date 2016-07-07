//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "err/macro.h"
#include <gtest/gtest.h>

TEST(MacroTest, PP_STRINGIZE)
{
    EXPECT_STREQ("foo", PP_STRINGIZE(foo));
}

TEST(MacroTest, PP_NARG)
{
    EXPECT_EQ(0, PP_NARG());
    EXPECT_EQ(1, PP_NARG(a));
    EXPECT_EQ(2, PP_NARG(a, b));
    EXPECT_EQ(3, PP_NARG(a, b, c));
    EXPECT_EQ(4, PP_NARG(a, b, c, d));
    EXPECT_EQ(5, PP_NARG(a, b, c, d, e));
}
