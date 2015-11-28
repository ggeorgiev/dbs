#pragma once

namespace err
{
inline void err_assert(bool expression)
{
    if (!expression)
        throw nullptr;
}
}

#if defined(NDEBUG)
#define ASSERT(X) ((void)0)
#else
#define ASSERT(X) err::err_assert(X)
#endif
