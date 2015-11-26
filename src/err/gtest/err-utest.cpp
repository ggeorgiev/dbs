#include "err/err.h"
#include "gtest/err.h"

#include "gtest/gtest.h"

ECode return0()
{
    EHRET(kExpected);
}

TEST(ErrTest, return0)
{
    auto code = return0();
    ASSERT_STREQ("kExpected(1)\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:8: @ return0\n", err::gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(err::kExpected, code);
}

ECode return1()
{
    int a = 5;
    EHRET(kExpected, a);
}

TEST(ErrTest, return1)
{
    auto code = return1();
    ASSERT_STREQ("kExpected(1): a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:23: @ return1\n", err::gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(err::kExpected, code);
}

ECode return2()
{
    int a = 5;
    EHRET(kExpected, a, a);
}

TEST(ErrTest, return2)
{
    auto code = return2();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:38: @ return2\n", err::gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(err::kExpected, code);
}

ECode return3()
{
    int a = 5;
    EHRET(kExpected, a, a, a);
}

TEST(ErrTest, return3)
{
    auto code = return3();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:53: @ return3\n", err::gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(err::kExpected, code);
}

ECode return4()
{
    int a = 5;
    EHRET(kExpected, a, a, a, a);
}

TEST(ErrTest, return4)
{
    auto code = return4();
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:68: @ return4\n", err::gError->callstack().c_str());
    EH_RESET;
    ASSERT_EQ(err::kExpected, code);
}
