//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dp/handle.hpp"
#include "err/err.h"
#include <boost/functional/hash.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <tuple>
#include <unordered_map>

namespace dp
{
template <typename V>
class SolitaryContainer
{
public:
    typedef V Value;
    typedef Handle::ControllerSPtr Key;

    Handle::ControllerSPtr key(const HandleSPtr& handle)
    {
        return handle->controller();
    }

    Handle::ControllerSPtr holder(const Handle::ControllerSPtr& controller) const
    {
        return controller;
    }

    bool has(const Handle::ControllerSPtr& controller) const
    {
        ASSERT(controller != nullptr);
        ASSERT(!controller->expired());
        if (mController != controller)
            return false;

        for (const auto& depender : mDependencies)
            if (depender->expired())
                return false;

        return true;
    }

    Value get(const Handle::ControllerSPtr& controller)
    {
        ASSERT(has(controller));
        return mValue;
    }

    void put(const Handle::ControllerSPtr& controller, const Value& value)
    {
        mController = controller;
        mDependencies.clear();
        mValue = value;
    }

    void put(const Handle::ControllerSPtr& controller,
             const std::vector<Handle::ControllerSPtr>& dependencies,
             const Value& value)
    {
        mController = controller;
        mDependencies = dependencies;
        mValue = value;
    }

    size_t size() const
    {
        return mController == nullptr ? 0 : 1;
    }

    void clear()
    {
        mController = nullptr;
        mValue = Value();
    }

private:
    Handle::ControllerSPtr mController;
    std::vector<Handle::ControllerSPtr> mDependencies;
    Value mValue;
};

} // namespace dp