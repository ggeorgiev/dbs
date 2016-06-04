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
#include <unordered>

namespace dp
{
template <typename V, typename... Args>
class MapContainer
{
public:
    typedef V Value;
    typedef std::tuple<Handle::ControllerSPtr, Args...> Tuple;

    struct Hasher
    {
        struct HashItem
        {
            HashItem()
            {
            }

            template <typename Item>
            void operator()(const Item& item) const
            {
                std::hash<Item> hash;
                boost::hash_combine(mSeed, hash(item));
            }

            mutable std::size_t mSeed = 0;
        };

        std::size_t operator()(const Tuple& tuple) const
        {
            HashItem hashItem;
            boost::fusion::for_each(tuple, hashItem);
            return hashItem.mSeed;
        }
    };

    typedef unordered_map<Tuple,
                          std::pair<Value, std::vector<Handle::ControllerSPtr>>,
                          Hasher>
        Map;

    Tuple key(const HandleSPtr& handle, Args... args)
    {
        return Tuple(handle->controller(), args...);
    }

    typename Map::const_iterator holder(const Tuple& tuple) const
    {
        return mMap.find(tuple);
    }

    bool has(const typename Map::const_iterator& it)
    {
        if (it == mMap.end())
            return false;

        if (expired(it))
            return false;

        return true;
    }

    Value get(const typename Map::const_iterator& it)
    {
        return it->second.first;
    }

    void put(const Tuple& tuple, const Value& value)
    {
        mMap[tuple] = std::make_pair(value, std::vector<Handle::ControllerSPtr>());
    }

    void put(const Tuple& tuple,
             const std::vector<Handle::ControllerSPtr>& dependencies,
             const Value& value)
    {
        mMap[tuple] = std::make_pair(value, dependencies);
    }

    size_t size() const
    {
        return mMap.size();
    }

    void clear()
    {
        typename Map::iterator it = mMap.begin();
        while (it != mMap.end())
        {
            if (std::get<0>(it->first)->expired() || expired(it))
                it = mMap.erase(it);
            else
                ++it;
        }
    }

private:
    bool expired(const typename Map::const_iterator& it)
    {
        for (const auto& depender : it->second.second)
            if (depender->expired())
                return true;
        return false;
    }

    Map mMap;
};

} // namespace dp