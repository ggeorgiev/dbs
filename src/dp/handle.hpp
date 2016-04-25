//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace dp
{
template <typename T>
class Handle
{
public:
    typedef std::shared_ptr<Handle<T>> HandleSPtr;
    typedef std::weak_ptr<Handle<T>> HandleWPtr;

    class Controller
    {
    public:
        Controller(const HandleSPtr& handle)
            : mHandle(handle)
        {
        }

        bool expired() const
        {
            return mHandle.expired();
        }

    private:
        HandleWPtr mHandle;
    };

    typedef std::shared_ptr<Controller> ControllerSPtr;

    static HandleSPtr create(const std::shared_ptr<T>& object)
    {
        struct make_shared_enabler : public Handle<T>
        {
        };

        auto handle = std::make_shared<make_shared_enabler>();
        handle->mController = std::make_shared<Controller>(handle);
        handle->mObject = object;
        return handle;
    }

    Handle(const Handle&) = delete;
    Handle(Handle&&) = delete;

    ~Handle()
    {
        mObject->clear();
    }

    ControllerSPtr controller()
    {
        return mController;
    }

private:
    Handle()
    {
    }

    ControllerSPtr mController;
    std::shared_ptr<T> mObject;
};
} // namespace dp
