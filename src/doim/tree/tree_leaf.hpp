//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"

namespace doim
{
template <typename T, typename TN, typename... Args>
class TreeLeaf : public Element<T, shared_ptr<TN>, Args...>
{
public:
    TreeLeaf(const shared_ptr<TN>& ancestor, const Args&... args)
        : Element<T, shared_ptr<TN>, Args...>(ancestor, args...)
    {
        ASSERT(ancestor->isUnique());
    }

    const shared_ptr<TN>& ancestor() const
    {
        return std::get<0>(Element<T, shared_ptr<TN>, Args...>::mArgs);
    }
};
}
