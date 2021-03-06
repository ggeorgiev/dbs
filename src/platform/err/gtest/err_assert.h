//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include "err/gtest/err.h"

#if defined(NDEBUG)
#define ASSERT_ASSERT(expression) ((void)0)
#define ASSERT_EHASSERT(expression) ((void)0)
#else
#define ASSERT_ASSERT(expression) ASSERT_THROW(expression, std::runtime_error)
#define ASSERT_EHASSERT(expression)    \
    do                                 \
    {                                  \
        ECode code = (expression);     \
        ASSERT_EQ(err::kAssert, code); \
        err::gError.reset();           \
    } while (false)
#endif
