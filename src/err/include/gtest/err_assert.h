#pragma once

#include "err/err.h"
#include "gtest/err.h"

#if defined(NDEBUG)
#define ASSERT_ASSERT(expression) ((void)0)
#define ASSERT_EHASSERT(expression) ((void)0)
#else
#define ASSERT_ASSERT(expression) ASSERT_THROW(expression, void*)
#define ASSERT_EHASSERT(expression)    \
    do                                 \
    {                                  \
        ECode code = (expression);     \
        ASSERT_EQ(err::kAssert, code); \
        delete err::gError;            \
        err::gError = NULL;            \
    } while (false)
#endif
