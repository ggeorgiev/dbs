//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_program.h"
#include "doim/db/db_key.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/location.hpp"
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

class Manager : public ManagerObjectMixin<DbKey>, public ManagerObjectSetMixin<FsFile>
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

    // Obtain an unique location.
    LocationSPtr obtainLocation(const LocationSPtr& base, const string_view& location)
    {
        return FsDirectory::obtain(base, location);
    }

    using ManagerObjectMixin<FsFile>::unique;
    using ManagerObjectMixin<FsFile>::isUnique;
    using ManagerObjectMixin<FsFile>::find;

    FsFileSPtr findFile(const FsDirectorySPtr& base, const string_view& file);

    FsFileSPtr obtainFile(const FsDirectorySPtr& base, const string_view& file);

    using ManagerObjectSetMixin<FsFile>::unique;

    using ManagerObjectMixin<DbKey>::unique;
    using ManagerObjectMixin<DbKey>::isUnique;

private:
    unordered_map<FsFileSPtr, CxxHeaderSPtr> mFile2CxxHeader;
};

template <typename T, typename... Args>
shared_ptr<T> unique(const Args&... args)
{
    return gManager->unique(T::make(args...));
}
}
