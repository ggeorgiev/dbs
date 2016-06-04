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

    shared_ptr<T> firstCommonAncestor(const shared_ptr<T>& other) const
    {
        if (other == nullptr)
            return nullptr;

        shared_ptr<T> line1 = other;

        while (line1->level() > level())
            line1 = line1->ancestor();

        if (line1.get() == this)
            return line1;

        const TreeNode* line2 = this;
        while (line1->level() < line2->level())
            line2 = line2->ancestor().get();

        while (line1.get() != line2)
        {
            line1 = line1->ancestor();
            line2 = line2->ancestor().get();
        }

        return line1;
    }

private:
    size_t mLevel = 1;
};
}
