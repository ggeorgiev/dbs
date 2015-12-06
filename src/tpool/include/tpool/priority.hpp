//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"

#include <boost/optional.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <unordered_map>

namespace tpool
{
class Priority;
typedef std::shared_ptr<Priority> PrioritySPtr;

class Priority
{
public:
    typedef std::unordered_map<PrioritySPtr, int> Map;

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

        int value = dependee->value();

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
            if (minDependee->second < value)
                value = minDependee->second;

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
