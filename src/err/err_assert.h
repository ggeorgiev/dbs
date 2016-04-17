#pragma once

#include "err/macro.h"
#include <iostream>

namespace err
{
inline void err_assert(bool condition, const char* msg)
{
    if (condition)
        return;

    throw std::runtime_error(msg);
}
}

#if defined(NDEBUG)
#define ASSERT(X) ((void)0)
#else
#define ASSERT(X) \
    err::err_assert(X, __FILE__ ":" PP_STRINGIZE(__LINE__) ": \"" PP_STRINGIZE(X) "\"")
#endif
