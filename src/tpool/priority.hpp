//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include <boost/optional.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered>

namespace tpool
{
class Priority;
typedef shared_ptr<Priority> PrioritySPtr;

class Priority
{
public:
    typedef unordered_map<PrioritySPtr, int> Map;

    Priority()
        : mValue(std::numeric_limits<int>::max())
    {
    }
    explicit Priority(const int value)
        : mValue(value)
        , mExplicitValue(value)
    {
    }

    // Updates current priority with an dependee
    bool update(const int value)
    {
        auto explicitValue = mExplicitValue.value_or(std::numeric_limits<int>::max());
        mExplicitValue = value;

        if (explicitValue >= value)
            return updateValue(value);

        return recomputeValue();
    }

    // Updates current priority with an dependee
    bool update(const PrioritySPtr& dependee)
    {
        ASSERT(dependee != nullptr);

        auto it = mDependees.find(dependee);

        // Every priority should exceed all its dependees
        int value = dependee->value() - 1;

        // We cannot find this dependee.
        if (it == mDependees.end())
        {
            mDependees[dependee] = value;
            return updateValue(value);
        }

        // We already have this dependee, but it did not change its value.
        if (it->second == value)
            return false;

        it->second = value;
        return recomputeValue();
    }

    int value()
    {
        return mValue;
    }

private:
    bool recomputeValue()
    {
        auto minDependee =
            std::min_element(mDependees.begin(),
                             mDependees.end(),
                             [](const Map::value_type& p1, const Map::value_type& p2) {
                                 return p1.second < p2.second;
                             });

        auto value = mExplicitValue.value_or(std::numeric_limits<int>::max());

        if (minDependee != mDependees.end())
        {
            int dependee = minDependee->second;
            if (dependee < value)
                value = dependee;
        }

        if (mValue == value)
            return false;

        mValue = value;
        return true;
    }

    bool updateValue(int value)
    {
        if (mValue < value)
            return false;

        mValue = value;
        return true;
    }

    Map mDependees;
    int mValue;
    boost::optional<int> mExplicitValue;
};

} // namespace tpool
