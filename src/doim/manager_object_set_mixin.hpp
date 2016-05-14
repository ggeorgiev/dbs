//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/manager_object_mixin.hpp"
#include "err/err_assert.h"
#include <boost/functional/hash/hash.hpp>
#include <algorithm>
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
template <typename T>
class ManagerObjectSetMixin : public ManagerObjectMixin<T>
{
public:
    typedef T MixinObject;
    typedef std::shared_ptr<MixinObject> MixinObjectSPtr;

    typedef unordered_set<MixinObjectSPtr> MixinObjectSet;
    typedef std::shared_ptr<MixinObjectSet> MixinObjectSetSPtr;

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

    using ManagerObjectMixin<T>::isUnique;

    MixinObjectSetSPtr unique(const MixinObjectSetSPtr& objects)
    {
        if (objects == nullptr)
            return objects;

        for (const auto& object : *objects)
            ASSERT(isUnique(object));

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
}
