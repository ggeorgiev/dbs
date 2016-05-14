//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace dp
{
class Handle;
typedef shared_ptr<Handle> HandleSPtr;
typedef weak_ptr<Handle> HandleWPtr;

class Handle
{
public:
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

    typedef shared_ptr<Controller> ControllerSPtr;
    typedef std::function<void()> ClearFunction;

    static HandleSPtr create(const ClearFunction& function)
    {
        struct make_shared_enabler : public Handle
        {
        };

        auto handle = std::make_shared<make_shared_enabler>();
        handle->mController = std::make_shared<Controller>(handle);
        handle->mClearFunction = function;
        return handle;
    }

    Handle(const Handle&) = delete;
    Handle(Handle&&) = delete;

    ~Handle()
    {
        if (mClearFunction != nullptr)
            mClearFunction();
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
    ClearFunction mClearFunction;
};
} // namespace dp
