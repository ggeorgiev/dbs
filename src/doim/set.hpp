//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/manager_object_mixin.hpp"
#include "err/err_assert.h"
#include <boost/functional/hash/hash.hpp>
#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <memory>
#include <string_view>
#include <typeindex>
#include <unordered>
#include <unordered_map>
#include <utility>
#include <vector>
#include <stddef.h>

namespace doim
{
template <typename T, typename S>
class SetManagerObjectMixin
{
public:
    typedef T MixinObject;
    typedef shared_ptr<MixinObject> MixinObjectSPtr;

    typedef S MixinObjectSet;
    typedef shared_ptr<MixinObjectSet> MixinObjectSetSPtr;

    struct Hasher
    {
        std::size_t operator()(const MixinObjectSetSPtr& objects) const
        {
            std::vector<MixinObjectSPtr> vector;
            vector.reserve(objects->size());
            vector.insert(vector.begin(), objects->begin(), objects->end());
            std::sort(vector.begin(), vector.end());

            std::hash<MixinObjectSPtr> hash;

            std::size_t seed = 0;
            for (const auto& object : vector)
            {
                ASSERT(object != nullptr);
                boost::hash_combine(seed, hash(object));
            }

            return seed;
        }

        bool operator()(const MixinObjectSetSPtr& objects1,
                        const MixinObjectSetSPtr& objects2) const
        {
            return *objects1 == *objects2;
        }
    };

    MixinObjectSetSPtr unique(const MixinObjectSetSPtr& objects)
    {
        if (objects == nullptr)
            return objects;

        for (const auto& object : *objects)
            ASSERT(object->isUnique());

        boost::unique_lock<boost::shared_mutex> lock(mMixinObjectSetsMutex);
        return *mMixinObjectSets.insert(objects).first;
    }

    bool isUnique(const MixinObjectSetSPtr& object) const
    {
        return object == find(object);
    }

    MixinObjectSetSPtr find(const MixinObjectSetSPtr& object) const
    {
        if (object == nullptr)
            return object;

        boost::shared_lock<boost::shared_mutex> lock(mMixinObjectSetsMutex);
        const auto& it = mMixinObjectSets.find(object);
        if (it == mMixinObjectSets.end())
            return nullptr;
        return *it;
    }

protected:
    mutable boost::shared_mutex mMixinObjectSetsMutex;
    unordered_set<MixinObjectSetSPtr, Hasher, Hasher> mMixinObjectSets;
};

template <typename T>
class NewObjectSet : public enable_make_shared<NewObjectSet<T>>,
                     public std::unordered_set<shared_ptr<T>>
{
public:
    static shared_ptr<NewObjectSet<T>> unique(const shared_ptr<NewObjectSet<T>>& set)
    {
        return gManagerObjectSetMixin->unique(set);
    }

    static shared_ptr<NewObjectSet<T>> unique(
        const std::initializer_list<shared_ptr<T>>& list)
    {
        return gManagerObjectSetMixin->unique(
            enable_make_shared<NewObjectSet<T>>::make(list));
    }

    using std::unordered_set<shared_ptr<T>>::unordered_set;

    bool isUnique()
    {
        shared_ptr<NewObjectSet<T>> key(shared_ptr<NewObjectSet<T>>(), this);
        return gManagerObjectSetMixin->isUnique(key);
    }

    shared_ptr<NewObjectSet<T>> find()
    {
        shared_ptr<NewObjectSet<T>> key(shared_ptr<NewObjectSet<T>>(), this);
        return gManagerObjectSetMixin->find(key);
    }

    struct Hasher
    {
        std::size_t operator()(const shared_ptr<NewObjectSet<T>>& objects) const
        {
            std::vector<shared_ptr<T>> vector;
            vector.reserve(objects->size());
            vector.insert(vector.begin(), objects->begin(), objects->end());
            std::sort(vector.begin(), vector.end());

            std::hash<shared_ptr<T>> hash;

            std::size_t seed = 0;
            for (const auto& object : vector)
            {
                ASSERT(object != nullptr);
                boost::hash_combine(seed, hash(object));
            }

            return seed;
        }

        bool operator()(const shared_ptr<NewObjectSet<T>>& objects1,
                        const shared_ptr<NewObjectSet<T>>& objects2) const
        {
            return *objects1 == *objects2;
        }
    };

protected:
    static shared_ptr<SetManagerObjectMixin<T, NewObjectSet<T>>> gManagerObjectSetMixin;
};

template <typename T>
shared_ptr<SetManagerObjectMixin<T, NewObjectSet<T>>> NewObjectSet<
    T>::gManagerObjectSetMixin =
    std::make_shared<SetManagerObjectMixin<T, NewObjectSet<T>>>();
}
