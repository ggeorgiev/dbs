//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "err/err.h"
#include <gtest/gtest.h>
#include <str>

ECode return0()
{
    EHBan(kExpected);
}

TEST(ErrTest, return0)
{
    auto code = return0();
    EXPECT_STREQ("kExpected(1)\n", err::gError->message().c_str());
    EXPECT_STREQ("src/platform/err/gtest/err-utest.cpp:10: @ return0\n",
                 err::gError->callstack().c_str());
    EHEnsureClear;
    ASSERT_EQ(err::kExpected, code);
}

ECode return1()
{
    int a = 5;
    EHBan(kExpected, a);
}

TEST(ErrTest, return1)
{
    auto code = return1();
    EXPECT_STREQ("kExpected(1): a = 5\n", err::gError->message().c_str());
    EXPECT_STREQ("src/platform/err/gtest/err-utest.cpp:26: @ return1\n",
                 err::gError->callstack().c_str());
    EHEnsureClear;
    ASSERT_EQ(err::kExpected, code);
}

ECode return2()
{
    int a = 5;
    EHBan(kExpected, a, a);
}

TEST(ErrTest, return2)
{
    auto code = return2();
    EXPECT_STREQ("kExpected(1): a = 5, a = 5\n", err::gError->message().c_str());
    EXPECT_STREQ("src/platform/err/gtest/err-utest.cpp:42: @ return2\n",
                 err::gError->callstack().c_str());
    EHEnsureClear;
    ASSERT_EQ(err::kExpected, code);
}

ECode return3()
{
    int a = 5;
    EHBan(kExpected, a, a, a);
}

TEST(ErrTest, return3)
{
    auto code = return3();
    EXPECT_STREQ("kExpected(1): a = 5, a = 5, a = 5\n", err::gError->message().c_str());
    EXPECT_STREQ("src/platform/err/gtest/err-utest.cpp:58: @ return3\n",
                 err::gError->callstack().c_str());
    EHEnsureClear;
    ASSERT_EQ(err::kExpected, code);
}

ECode return4()
{
    int a = 5;
    EHBan(kExpected, a, a, a, a);
}

TEST(ErrTest, return4)
{
    auto code = return4();
    EXPECT_STREQ("kExpected(1): a = 5, a = 5, a = 5, a = 5\n",
                 err::gError->message().c_str());
    EXPECT_STREQ("src/platform/err/gtest/err-utest.cpp:74: @ return4\n",
                 err::gError->callstack().c_str());
    EHEnsureClear;
    ASSERT_EQ(err::kExpected, code);
}
