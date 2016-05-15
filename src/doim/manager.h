//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/db/db_key.h"
#include "doim/manager_object_mixin.hpp"
#include "doim/manager_object_set_mixin.hpp"
#include "im/initialization_manager.hpp"
#include <shared_ptr>
#include <str>
#include <string_view>
#include <unordered>

namespace doim
{
class Manager;
typedef shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;

class Manager : public ManagerObjectMixin<DbKey>
{
public:
    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    static constexpr int object_rank()
    {
        return rank() + im::InitializationManager::step();
    }

    template <typename T, typename... Args>
    static shared_ptr<T> global(const Args&... args, shared_ptr<T>& object)
    {
        auto fn = [&object]() -> bool {
            object = gManager->unique(object);
            return true;
        };
        im::InitializationManager::subscribe<shared_ptr<T>>(object_rank(), fn, nullptr);
        return std::make_shared<T>(args...);
    }

    using ManagerObjectMixin<DbKey>::unique;
    using ManagerObjectMixin<DbKey>::isUnique;
};

template <typename T, typename... Args>
shared_ptr<T> unique(const Args&... args)
{
    return gManager->unique(T::make(args...));
}
}
