//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//
//
// Err is an error handling system that allows call stack tracking.
//

#pragma once

#include <functional>

namespace dtl
{
template <typename L>
class Defer
{
public:
    Defer(L&& lambda)
        : mLambda(std::forward<L>(lambda))
    {
    }

    ~Defer()
    {
        mLambda();
    }

    L mLambda;
};

template <typename L>
Defer<L> deferLambda(L&& lambda)
{
    return Defer<L>(std::forward<L>(lambda));
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x) DEFER_2(x, __COUNTER__)
#define defer(code) auto DEFER_3(_defer_) = dtl::deferLambda([&]() { code; });

} // namespace Impl