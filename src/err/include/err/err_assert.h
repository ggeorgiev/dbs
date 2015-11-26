#pragma once

namespace err
{
inline void assert(bool expression)
{
    if (!expression)
        throw nullptr;
}
}

#if defined(NDEBUG)
#define ASSERT(X) ((void)0)
#else
#define ASSERT(X) err::assert(X)
#endif
