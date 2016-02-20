//  Copyright © 2015-1016 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <experimental/string_view>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace task
{
template <typename T, typename... Args>
class Base
{
public:
    typedef std::tuple<Args...> Tuple;

    Base(const Args&... args)
        : mArgs(args...)
    {
    }

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

        std::size_t operator()(const std::shared_ptr<T>& object) const
        {
            HashItem hashItem;
            boost::fusion::for_each(object->mArgs, hashItem);
            return hashItem.mSeed;
        }

        bool operator()(const std::shared_ptr<T>& object1,
                        const std::shared_ptr<T>& object2) const
        {
            return object1->mArgs == object2->mArgs;
        }
    };

protected:
    Tuple mArgs;
};
}
