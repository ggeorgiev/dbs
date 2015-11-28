#include "err/err.h"
#include "err/err_cppformat.h"

#include <gtest/gtest.h>

#include <sstream>

ECode return0()
{
    EHBan(kExpected);
}

TEST(ErrTest, return0)
{
    auto code = return0();
    ASSERT_STREQ("kExpected(1)\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:10: @ return0\n", err::gError->callstack().c_str());
    EHReset;
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
    ASSERT_STREQ("kExpected(1): a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:25: @ return1\n", err::gError->callstack().c_str());
    EHReset;
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
    ASSERT_STREQ("kExpected(1): a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:40: @ return2\n", err::gError->callstack().c_str());
    EHReset;
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
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:55: @ return3\n", err::gError->callstack().c_str());
    EHReset;
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
    ASSERT_STREQ("kExpected(1): a = 5, a = 5, a = 5, a = 5\n", err::gError->message().c_str());
    ASSERT_STREQ("src/err/gtest/err-utest.cpp:70: @ return4\n", err::gError->callstack().c_str());
    EHReset;
    ASSERT_EQ(err::kExpected, code);
}
