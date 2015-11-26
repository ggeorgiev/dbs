#pragma once

#if defined(NDEBUG)
#define EXPECT_ASSERT(X) ((void)0)
#else
#define EXPECT_ASSERT(X) ASSERT_THROW(X, void*)
#endif
