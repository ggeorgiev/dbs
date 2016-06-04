//  Copyright © 2016 George Georgiev. All rights reserved.
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
template <typename V>
class SimpleMemoization
{
public:
    typedef V Value;
    typedef std::function<Value()> CalculateFunction;

    SimpleMemoization()
    {
    }

    Value get(const CalculateFunction& calculate)
    {
        boost::upgrade_lock<boost::shared_mutex> shared_lock(mContainerMutex);
        if (mHas)
            return mValue;

        // This is controversial but we prefer to block any other access while
        // calculating. This will prevent double calculation.
        boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(shared_lock);
        mHas = true;
        return mValue = calculate();
    }

    void clear()
    {
        boost::unique_lock<boost::shared_mutex> lock(mContainerMutex);
        mHas = false;
        mValue = Value();
    }

private:
    mutable boost::shared_mutex mContainerMutex;
    bool mHas = false;
    Value mValue;
};
} // namespace dp