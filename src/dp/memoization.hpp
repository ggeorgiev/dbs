//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dp/handle.hpp"
#include "err/err.h"
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <tuple>
#include <unordered_map>

namespace dp
{
template <template <typename, typename...> class C, typename V, typename... Args>
class Memoization
{
public:
    typedef C<V, Args...> Container;
    typedef typename Container::Value Value;

    typedef std::function<Value(Args..., std::vector<Handle::ControllerSPtr>&)>
        CalculateFunction;

    Memoization()
    {
    }

    Value get(const HandleSPtr& handle, Args... args, const CalculateFunction& calculate)
    {
        const auto& key = mContainer.key(handle, args...);

        boost::upgrade_lock<boost::shared_mutex> shared_lock(mContainerMutex);
        const auto& holder = mContainer.holder(key);
        if (mContainer.has(holder))
            return mContainer.get(holder);

        // This is controversial but we prefer to block any other access while
        // calculating. This will prevent double calculation.
        boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(shared_lock);

        std::vector<Handle::ControllerSPtr> dependencies;
        const Value& value = calculate(args..., dependencies);
        mContainer.put(key, dependencies, value);

        return value;
    }

    size_t size() const
    {
        boost::shared_lock<boost::shared_mutex> lock(mContainerMutex);
        return mContainer.size();
    }

    void clear()
    {
        boost::unique_lock<boost::shared_mutex> lock(mContainerMutex);
        mContainer.clear();
    }

private:
    mutable boost::shared_mutex mContainerMutex;
    Container mContainer;
};
} // namespace dp