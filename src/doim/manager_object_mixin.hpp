//  Copyright © 2015-1016 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/functional/hash/hash.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <algorithm>
#include <experimental/string_view>
#include <iosfwd>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <stddef.h>

namespace doim
{
template <typename T>
class ManagerObjectMixin
{
public:
    typedef T MixinObject;
    typedef std::shared_ptr<MixinObject> MixinObjectSPtr;
    typedef typename MixinObject::Hasher Hasher;
    typedef std::unordered_set<MixinObjectSPtr, Hasher, Hasher> Set;

    MixinObjectSPtr unique(const MixinObjectSPtr& object)
    {
        if (object == nullptr)
            return object;

        std::pair<typename Set::iterator, bool> result;
        {
            boost::unique_lock<boost::shared_mutex> lock(mMixinObjectsMutex);
            result = mMixinObjects.insert(object);
        }

        if (result.second)
            (*result.first)->finally();

        return *result.first;
    }

    bool isUnique(const MixinObjectSPtr& object) const
    {
        return object == find(object);
    }

    MixinObjectSPtr find(const MixinObjectSPtr& object) const
    {
        if (object == nullptr)
            return object;

        boost::shared_lock<boost::shared_mutex> lock(mMixinObjectsMutex);
        const auto& it = mMixinObjects.find(object);
        if (it == mMixinObjects.end())
            return nullptr;
        return *it;
    }

protected:
    mutable boost::shared_mutex mMixinObjectsMutex;
    Set mMixinObjects;
};
}
