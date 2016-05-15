//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/manager_object_mixin.hpp"
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <memory>
#include <shared_ptr>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace doim
{
template <typename T, typename... Args>
class Base : public enable_make_shared<T>
{
public:
    typedef std::tuple<Args...> Tuple;

    static shared_ptr<T> unique(const Args&... args)
    {
        return gManagerObjectMixin->unique(enable_make_shared<T>::make(args...));
    }

    Base(const Args&... args)
        : mArgs(args...)
    {
    }

    bool isUnique()
    {
        std::shared_ptr<T> key(std::shared_ptr<T>(), static_cast<T*>(this));
        return gManagerObjectMixin->isUnique(key);
    }

    void finally()
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

        std::size_t operator()(const shared_ptr<T>& object) const
        {
            HashItem hashItem;
            boost::fusion::for_each(object->mArgs, hashItem);
            return hashItem.mSeed;
        }

        bool operator()(const shared_ptr<T>& object1, const shared_ptr<T>& object2) const
        {
            return object1->mArgs == object2->mArgs;
        }
    };

protected:
    static shared_ptr<ManagerObjectMixin<T>> gManagerObjectMixin;
    Tuple mArgs;
};

template <typename T, typename... Args>
shared_ptr<ManagerObjectMixin<T>> Base<T, Args...>::gManagerObjectMixin =
    std::make_shared<ManagerObjectMixin<T>>();
}
