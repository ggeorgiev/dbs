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

namespace task
{
template <typename T, typename... Args>
class ElementManager
{
public:
    typedef T Element;
    typedef shared_ptr<Element> ElementSPtr;

    typedef typename Element::Hasher Hasher;

    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    ElementSPtr valid(const ElementSPtr& element)
    {
        if (element == nullptr)
            return element;

        std::unique_lock<std::mutex> lock(mElementMutex);
        return *mElements.insert(element).first;
    }

    ElementSPtr unique(const ElementSPtr& element)
    {
        if (element == nullptr)
            return element;

        std::unique_lock<std::mutex> lock(mElementMutex);
        return *mElements.insert(element).first;
    }

    void erase(const ElementSPtr& element)
    {
        if (element == nullptr)
            return;

        std::unique_lock<std::mutex> lock(mElementMutex);
        mElements.erase(element);
    }

protected:
    std::mutex mElementMutex;
    unordered_set<ElementSPtr, Hasher, Hasher> mElements;
};
}
