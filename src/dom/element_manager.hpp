//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/generic/object.h"
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

namespace dom
{
template <typename T>
class ElementManager
{
public:
    typedef T Element;
    typedef shared_ptr<Element> ElementSPtr;
    typedef unordered_map<doim::ObjectSPtr, ElementSPtr> Map;

    static constexpr int rank()
    {
        // TODO: This must be higher than the objects rank
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    ElementSPtr obtain(const doim::ObjectSPtr& object)
    {
        if (object == nullptr)
            return nullptr;

        boost::upgrade_lock<boost::shared_mutex> shared_lock(mElementsMutex);
        const auto& it = mElements.find(object);
        if (it != mElements.end())
            return it->second;

        boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(shared_lock);
        const auto& element = Element::make();
        mElements[object] = element;
        return element;
    }

    ElementSPtr find(const doim::ObjectSPtr& object) const
    {
        if (object == nullptr)
            return nullptr;

        boost::shared_lock<boost::shared_mutex> lock(mElementsMutex);
        const auto& it = mElements.find(object);
        if (it == mElements.end())
            return nullptr;
        return it->second;
    }

protected:
    mutable boost::shared_mutex mElementsMutex;
    Map mElements;
};
}
