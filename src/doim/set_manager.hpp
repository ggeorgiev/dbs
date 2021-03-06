//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include <boost/functional/hash/hash.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <memory>
#include <string_view>
#include <typeindex>
#include <unordered>
#include <utility>
#include <vector>
#include <stddef.h>

namespace doim
{
template <typename T, typename S>
class SetManager
{
public:
    typedef T Object;
    typedef shared_ptr<Object> ObjectSPtr;

    typedef S ObjectSet;
    typedef shared_ptr<ObjectSet> ObjectSetSPtr;

    static constexpr int rank()
    {
        return T::rank() + T::levels() * im::InitializationManager::step();
    }

    struct Hasher
    {
        std::size_t operator()(const ObjectSetSPtr& objects) const
        {
            std::uintptr_t x = 0;
            for (const auto& object : *objects)
                x ^= (std::uintptr_t)object.get();

            std::hash<uintptr_t> hash;
            return hash(x);
        }

        bool operator()(const ObjectSetSPtr& objects1,
                        const ObjectSetSPtr& objects2) const
        {
            return *objects1 == *objects2;
        }
    };

    ObjectSetSPtr unique(const ObjectSetSPtr& objects)
    {
        if (objects == nullptr)
            return objects;

        for (const auto& object : *objects)
            ASSERT(object->isUnique());

        boost::unique_lock<boost::shared_mutex> lock(mObjectSetsMutex);
        return *mObjectSets.insert(objects).first;
    }

    bool isUnique(const ObjectSetSPtr& object) const
    {
        return object == find(object);
    }

    ObjectSetSPtr find(const ObjectSetSPtr& object) const
    {
        if (object == nullptr)
            return object;

        boost::shared_lock<boost::shared_mutex> lock(mObjectSetsMutex);
        const auto& it = mObjectSets.find(object);
        if (it == mObjectSets.end())
            return nullptr;
        return *it;
    }

protected:
    mutable boost::shared_mutex mObjectSetsMutex;
    unordered_set<ObjectSetSPtr, Hasher, Hasher> mObjectSets;
};
}
