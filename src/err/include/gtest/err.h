#pragma once

#include "err/err.h"

#include "gtest/gtest.h"

#define ASSERT_BANNED(expected, expression)      \
    do                                           \
    {                                            \
        EXPECT_NE(err::expected, err::kSuccess); \
                                                 \
        ECode code = (expression);               \
        EXPECT_EQ(err::expected, code);          \
        EHEnsureClear;                           \
    } while (false)

#define ASSERT_OKAY(expression)                                                     \
    do                                                                              \
    {                                                                               \
        ECode code = (expression);                                                  \
        EXPECT_EQ(err::kSuccess, code) << "There is an error: <" << err::name(code) \
                                       << " | " << code << ">" << std::endl         \
                                       << err::gError->message() << std::endl       \
                                       << err::gError->callstack();                 \
        EHEnsureClear;                                                              \
    } while (false)
