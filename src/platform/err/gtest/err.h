//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"

#include <gtest/gtest.h>

#define ASSERT_BANNED(expected, expression)                                              \
    do                                                                                   \
    {                                                                                    \
        ASSERT_NE(err::expected, err::kSuccess);                                         \
                                                                                         \
        ECode code = (expression);                                                       \
        ASSERT_NE(code, err::kSuccess);                                                  \
        EXPECT_EQ(err::expected, code) << "The error is different: <" << err::name(code) \
                                       << " | " << code << ">" << std::endl              \
                                       << err::gError->message() << std::endl            \
                                       << err::gError->callstack();                      \
        EHEnsureClear;                                                                   \
        ASSERT_EQ(err::expected, code);                                                  \
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
        ASSERT_EQ(err::kSuccess, code);                                             \
    } while (false)
