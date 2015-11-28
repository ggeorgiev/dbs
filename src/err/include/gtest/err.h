#pragma once

#include "err/err.h"

#include "gtest/gtest.h"

#define ASSERT_BANNED(expected, expression)      \
    do                                           \
    {                                            \
        ASSERT_NE(err::expected, err::kSuccess); \
                                                 \
        ECode code = (expression);               \
        ASSERT_EQ(err::expected, code);          \
        EHReset;                                 \
    } while (false)

#define ASSERT_OKAY(expression)         \
    do                                  \
    {                                   \
        ECode code = (expression);      \
        ASSERT_EQ(err::kSuccess, code); \
    } while (false)
