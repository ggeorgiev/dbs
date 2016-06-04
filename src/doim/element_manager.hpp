//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "im/initialization_manager.hpp"
#include <boost/functional/hash/hash.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
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
class ElementManager
{
public:
    typedef T Element;
    typedef shared_ptr<Element> ElementSPtr;
    typedef typename Element::Hasher Hasher;
    typedef unordered_set<ElementSPtr, Hasher, Hasher> Set;

    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    ElementSPtr unique(const ElementSPtr& element)
    {
        if (element == nullptr)
            return element;

        {
            boost::upgrade_lock<boost::upgrade_mutex> shared_lock(mElementsMutex);
            const auto& it = mElements.find(element);
            if (it != mElements.end())
                return *it;

            boost::upgrade_to_unique_lock<boost::upgrade_mutex> unique_lock(shared_lock);
            mElements.insert(element);
        }

        element->finally();
        return element;
    }

    bool isUnique(const ElementSPtr& object) const
    {
        return object == find(object);
    }

    ElementSPtr find(const ElementSPtr& object) const
    {
        if (object == nullptr)
            return object;

        boost::shared_lock<boost::upgrade_mutex> lock(mElementsMutex);
        const auto& it = mElements.find(object);
        if (it == mElements.end())
            return nullptr;
        return *it;
    }

protected:
    mutable boost::upgrade_mutex mElementsMutex;
    Set mElements;
};
}
