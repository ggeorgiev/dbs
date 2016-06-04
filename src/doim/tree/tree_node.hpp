//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "err/err_assert.h"

namespace doim
{
template <typename T, typename... Args>
class TreeNode : public Element<T, shared_ptr<T>, Args...>
{
public:
    TreeNode(const shared_ptr<T>& ancestor, const Args&... args)
        : Element<T, shared_ptr<T>, Args...>(ancestor, args...)
        , mLevel(ancestor == nullptr ? 1 : ancestor->level() + 1)
    {
        ASSERT(ancestor->isUnique());
    }

    size_t level() const
    {
        return mLevel;
    }

    const shared_ptr<T>& ancestor() const
    {
        return std::get<0>(Element<T, shared_ptr<T>, Args...>::mArgs);
    }

private:
    size_t mLevel = 1;
};
}
