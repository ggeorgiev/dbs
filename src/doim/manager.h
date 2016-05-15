//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_program.h"
#include "doim/db/db_key.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/location.hpp"
#include "doim/manager_object_mixin.hpp"
#include "doim/manager_object_set_mixin.hpp"
#include "doim/sys/argument.h"
#include "doim/tag/tag.h"
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

class Manager : public ManagerObjectMixin<DbKey>,
                public ManagerObjectSetMixin<FsDirectory>,
                public ManagerObjectSetMixin<FsFile>,
                public ManagerObjectSetMixin<SysArgument>,
                public ManagerObjectSetMixin<Tag>
{
public:
    static constexpr int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    static constexpr int object_initialization_rank()
    {
        return initialization_rank() + im::InitializationManager::rank_step();
    }

    template <typename T, typename... Args>
    static shared_ptr<T> global(const Args&... args, shared_ptr<T>& object)
    {
        auto fn = [&object]() -> bool {
            object = gManager->unique(object);
            return true;
        };
        im::InitializationManager::subscribe<shared_ptr<T>>(object_initialization_rank(),
                                                            fn,
                                                            nullptr);
        return std::make_shared<T>(args...);
    }

    // Obtain an unique location.
    LocationSPtr obtainLocation(const LocationSPtr& base, const string_view& location)
    {
        return obtainDirectory(base, location);
    }

    using ManagerObjectMixin<FsDirectory>::unique;
    using ManagerObjectMixin<FsDirectory>::isUnique;
    using ManagerObjectMixin<FsDirectory>::find;

    using ManagerObjectSetMixin<FsDirectory>::unique;
    using ManagerObjectSetMixin<FsDirectory>::isUnique;
    using ManagerObjectSetMixin<FsDirectory>::find;

    FsDirectorySPtr findDirectory(const FsDirectorySPtr& base,
                                  const string_view& directory);
    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    FsDirectorySPtr obtainCorrespondingDirectory(const FsDirectorySPtr& directory,
                                                 const FsDirectorySPtr& fromDirectory,
                                                 const FsDirectorySPtr& toDirectory);

    using ManagerObjectMixin<FsFile>::unique;
    using ManagerObjectMixin<FsFile>::isUnique;
    using ManagerObjectMixin<FsFile>::find;

    FsFileSPtr findFile(const FsDirectorySPtr& base, const string_view& file);

    FsFileSPtr obtainFile(const FsDirectorySPtr& base, const string_view& file);

    using ManagerObjectSetMixin<FsFile>::unique;

    using ManagerObjectMixin<DbKey>::unique;
    using ManagerObjectMixin<DbKey>::isUnique;

    using ManagerObjectMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::isUnique;

    SysArgumentSPtr obtainArgument(const string& value)
    {
        return unique(doim::SysArgument::make(value));
    }

    SysArgumentSetSPtr obtainArguments(const SysArgumentSPtr argument)
    {
        auto arguments = SysArgumentSet::make();
        arguments->insert(argument);
        return unique(arguments);
    }

    using ManagerObjectMixin<Tag>::unique;
    using ManagerObjectMixin<Tag>::find;
    using ManagerObjectSetMixin<Tag>::unique;

private:
    unordered_map<FsFileSPtr, CxxHeaderSPtr> mFile2CxxHeader;
};

template <typename T, typename... Args>
shared_ptr<T> unique(const Args&... args)
{
    return gManager->unique(T::make(args...));
}

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory);
std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories);
std::ostream& operator<<(std::ostream& out, const CxxHeader& header);
}
