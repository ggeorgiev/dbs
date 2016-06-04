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
    TreeNode(const shared_ptr<T>& parent, const Args&... args)
        : Element<T, shared_ptr<T>, Args...>(parent, args...)
        , mLevel(parent == nullptr ? 1 : parent->level() + 1)
    {
        ASSERT(parent->isUnique());
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
