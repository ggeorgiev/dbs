//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/manager_object_mixin.hpp"
#include "im/initialization_manager.hpp"
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

    static shared_ptr<T> unique(const shared_ptr<T>& object)
    {
        return gManagerObjectMixin->unique(object);
    }

    static int constexpr rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    static shared_ptr<T> global(const Args&... args, shared_ptr<T>& object)
    {
        auto fn = [&object]() -> bool {
            object = gManagerObjectMixin->unique(object);
            return true;
        };
        im::InitializationManager::subscribe<
            shared_ptr<T>>(ManagerObjectMixin<T>::rank() +
                               im::InitializationManager::step(),
                           fn,
                           nullptr);
        return std::make_shared<T>(args...);
    }

    Base(const Args&... args)
        : mArgs(args...)
    {
    }

    bool isUnique()
    {
        shared_ptr<T> key(shared_ptr<T>(), static_cast<T*>(this));
        return gManagerObjectMixin->isUnique(key);
    }

    shared_ptr<T> find()
    {
        shared_ptr<T> key(shared_ptr<T>(), static_cast<T*>(this));
        return gManagerObjectMixin->find(key);
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
    im::InitializationManager::subscribe(gManagerObjectMixin);
}
