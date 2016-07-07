//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "log/hex.h"
#include <gtest/gtest.h>
#include <str>

TEST(HexTest, simple)
{
    struct Test
    {
        string input;
        string dump;
    };

    const auto zero = string(5, 0);

    Test tests[]{
        Test{.input = zero, .dump = "0000 : .....            00  00  00  00  00 \n"},
        Test{.input = "dumptest",
             .dump = "0000 : dumptest         64  75  6D  70  74  65  73  74 \n"},
        Test{.input = "dumptestfullline",
             .dump = "0000 : dumptestfullline 64  75  6D  70  74  65  73  74  "
                     " 66  75  6C  6C  6C  69  6E  65 \n"},
        Test{.input = "dumptest2partiallines",
             .dump = "0000 : dumptest2partial 64  75  6D  70  74  65  73  74  "
                     " 32  70  61  72  74  69  61  6C \n"
                     "0010 : lines            6C  69  6E  65  73 \n"},
        Test{.input = "\n\r\t\v\xff",
             .dump = "0000 : .....            0A  0D  09  0B  FF \n"},
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "Input: \"" << test.input << "\"");
        const auto& result = dbslog::hexdump(test.input);
        EXPECT_EQ(test.dump, result);
    }
}
