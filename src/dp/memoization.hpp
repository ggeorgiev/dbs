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
template <typename V, typename... Args>
class Memoization
{
public:
    typedef V Value;
    typedef Handle<Memoization<V, Args...>> Handle;
    typedef std::shared_ptr<Handle> HandleSPtr;

    typedef typename Handle::Controller Controller;
    typedef typename Handle::ControllerSPtr ControllerSPtr;

    typedef std::tuple<ControllerSPtr, Args...> Tuple;

    Memoization()
        : mGeneration(0)
        , mlastCleanGeneration(0)
    {
    }

    Value get(const Tuple& tuple)
    {
        boost::shared_lock<boost::shared_mutex> lock(mMapMutex);
        auto it = mMap.find(tuple);
        if (it == mMap.end())
            return Value();
        return it->second;
    }

    void put(const Tuple& tuple, const Value& value)
    {
        boost::unique_lock<boost::shared_mutex> lock(mMapMutex);
        mMap[tuple] = value;
        ++mGeneration;
    }

    size_t size() const
    {
        boost::shared_lock<boost::shared_mutex> lock(mMapMutex);
        return mMap.size();
    }

    void clear()
    {
        boost::unique_lock<boost::shared_mutex> lock(mMapMutex);
        if (mGeneration == mlastCleanGeneration)
            return;

        typename Map::iterator it = mMap.begin();
        while (it != mMap.end())
        {
            if (std::get<0>(it->first)->expired())
                it = mMap.erase(it);
            else
                ++it;
        }

        mlastCleanGeneration = mGeneration;
    }

private:
    struct Hasher
    {
        struct HashItem
        {
            HashItem()
                : mSeed(0)
            {
            }

            template <typename Item>
            void operator()(const Item& item) const
            {
                std::hash<Item> hash;
                boost::hash_combine(mSeed, hash(item));
            }

            mutable std::size_t mSeed;
        };

        std::size_t operator()(const Tuple& tuple) const
        {
            HashItem hashItem;
            boost::fusion::for_each(tuple, hashItem);
            return hashItem.mSeed;
        }
    };

    typedef std::unordered_map<Tuple, Value, Hasher> Map;

    mutable boost::shared_mutex mMapMutex;
    Map mMap;
    size_t mGeneration;
    size_t mlastCleanGeneration;
};
} // namespace dp
