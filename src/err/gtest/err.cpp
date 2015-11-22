#include "err/err.h"
#include "gtest/err.h"

#include "gtest/gtest.h"

ECode return0()
{
    EHRET(kExpected);
}

TEST(ErrTest, return0)
{
    auto err = return0();
    ASSERT_STREQ("kExpected(1)\n", gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err.cpp:8: @ return0\n", gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(kExpected, err);
}

ECode return1()
{
    int a = 5;
    EHRET(kExpected, a);
}

TEST(ErrTest, return1)
{
    auto err = return1();
    ASSERT_STREQ("kExpected(1): a = 5\n", gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err.cpp:23: @ return1\n", gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(kExpected, err);
}

ECode return2()
{
    int a = 5;
    EHRET(kExpected, a, a);
}

TEST(ErrTest, return2)
{
    auto err = return2();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5\n", gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err.cpp:38: @ return2\n", gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(kExpected, err);
}

ECode return3()
{
    int a = 5;
    EHRET(kExpected, a, a, a);
}

TEST(ErrTest, return3)
{
    auto err = return3();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5\n", gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err.cpp:53: @ return3\n", gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(kExpected, err);
}

ECode return4()
{
    int a = 5;
    EHRET(kExpected, a, a, a, a);
}

TEST(ErrTest, return4)
{
    auto err = return4();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5, a = 5\n", gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err.cpp:68: @ return4\n", gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(kExpected, err);
}
