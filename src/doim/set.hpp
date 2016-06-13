//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/set_manager.hpp"
#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include <boost/functional/hash/hash.hpp>
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
template <typename T>
class Set : public enable_make_shared<Set<T>>, public std::unordered_set<shared_ptr<T>>
{
public:
    static constexpr int rank()
    {
        return SetManager<T, Set<T>>::rank() + im::InitializationManager::step();
    }

    static shared_ptr<Set<T>> unique(const shared_ptr<Set<T>>& set)
    {
        return gManagerSet->unique(set);
    }

    static shared_ptr<Set<T>> unique(const std::initializer_list<shared_ptr<T>>& list)
    {
        return gManagerSet->unique(enable_make_shared<Set<T>>::make(list));
    }

    static shared_ptr<Set<T>> global(const std::vector<const shared_ptr<T>*> vector,
                                     shared_ptr<Set<T>>& set)
    {
        auto fn = [&set, vector]() -> bool {
            set = Set<T>::make();
            for (const auto& i : vector)
                set->insert(*i);
            set = Set<T>::unique(set);
            return true;
        };
        im::InitializationManager::subscribe<shared_ptr<Set<T>>>(Set<T>::rank(),
                                                                 fn,
                                                                 nullptr);
        return Set<T>::make();
    }

    using std::unordered_set<shared_ptr<T>>::unordered_set;

    bool isUnique()
    {
        shared_ptr<Set<T>> key(shared_ptr<Set<T>>(), this);
        return gManagerSet->isUnique(key);
    }

    shared_ptr<Set<T>> find()
    {
        shared_ptr<Set<T>> key(shared_ptr<Set<T>>(), this);
        return gManagerSet->find(key);
    }

    shared_ptr<Set<T>> combine(const shared_ptr<Set<T>> set)
    {
        ASSERT(set != nullptr && set->isUnique());
        auto result = Set<T>::make(*set);
        result->insert(std::unordered_set<shared_ptr<T>>::begin(),
                       std::unordered_set<shared_ptr<T>>::end());
        return gManagerSet->unique(result);
    }

    struct Hasher
    {
        std::size_t operator()(const shared_ptr<Set<T>>& objects) const
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

        bool operator()(const shared_ptr<Set<T>>& objects1,
                        const shared_ptr<Set<T>>& objects2) const
        {
            return *objects1 == *objects2;
        }
    };

protected:
    static shared_ptr<SetManager<T, Set<T>>> gManagerSet;
};

template <typename T>
shared_ptr<SetManager<T, Set<T>>> Set<T>::gManagerSet =
    im::InitializationManager::subscribe(gManagerSet);
}
