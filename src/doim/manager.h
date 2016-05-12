//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_program.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/db/db_key.h"
#include "doim/fs/fs_binary.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/location.hpp"
#include "doim/generic/object.h"
#include "doim/manager_object_mixin.hpp"
#include "doim/manager_object_set_mixin.hpp"
#include "doim/sys/argument.h"
#include "doim/sys/command.h"
#include "doim/sys/executable.h"
#include "doim/tag/tag.h"
#include "im/initialization_manager.hpp"
#include <experimental/string_view>
#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace doim
{
class Manager;
typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;

class Manager : public ManagerObjectMixin<CxxFile>,
                public ManagerObjectSetMixin<CxxHeader>,
                public ManagerObjectSetMixin<CxxIncludeDirectory>,
                public ManagerObjectSetMixin<CxxObjectFile>,
                public ManagerObjectSetMixin<CxxStaticLibrary>,
                public ManagerObjectMixin<CxxProgram>,
                public ManagerObjectMixin<DbKey>,
                public ManagerObjectMixin<FsBinary>,
                public ManagerObjectSetMixin<FsDirectory>,
                public ManagerObjectSetMixin<FsFile>,
                public ManagerObjectMixin<Object>,
                public ManagerObjectSetMixin<SysArgument>,
                public ManagerObjectMixin<SysCommand>,
                public ManagerObjectMixin<SysExecutable>,
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
    static std::shared_ptr<T> global(const Args&... args, std::shared_ptr<T>& object)
    {
        im::InitializationManager::subscribe<
            std::shared_ptr<T>>(object_initialization_rank(),
                                [&object]() -> bool {
                                    object = gManager->unique(object);
                                    return true;
                                },
                                nullptr);
        return std::make_shared<T>(args...);
    }

    // Obtain an unique location.
    LocationSPtr obtainLocation(const LocationSPtr& base,
                                const std::experimental::string_view& location)
    {
        return obtainDirectory(base, location);
    }

    using ManagerObjectMixin<Object>::unique;
    using ManagerObjectMixin<Object>::isUnique;
    using ManagerObjectMixin<Object>::find;

    // Obtain an unique object.
    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::EType type,
                            const std::experimental::string_view& object);

    using ManagerObjectMixin<FsBinary>::unique;
    using ManagerObjectMixin<FsBinary>::isUnique;
    using ManagerObjectMixin<FsBinary>::find;

    using ManagerObjectMixin<FsDirectory>::unique;
    using ManagerObjectMixin<FsDirectory>::isUnique;
    using ManagerObjectMixin<FsDirectory>::find;

    using ManagerObjectSetMixin<FsDirectory>::unique;
    using ManagerObjectSetMixin<FsDirectory>::isUnique;
    using ManagerObjectSetMixin<FsDirectory>::find;

    FsDirectorySPtr findDirectory(const FsDirectorySPtr& base,
                                  const std::experimental::string_view& directory);
    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::experimental::string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    FsDirectorySPtr obtainCorrespondingDirectory(const FsDirectorySPtr& directory,
                                                 const FsDirectorySPtr& fromDirectory,
                                                 const FsDirectorySPtr& toDirectory);

    using ManagerObjectMixin<FsFile>::unique;
    using ManagerObjectMixin<FsFile>::isUnique;
    using ManagerObjectMixin<FsFile>::find;

    FsFileSPtr findFile(const FsDirectorySPtr& base,
                        const std::experimental::string_view& file);

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::experimental::string_view& file);

    using ManagerObjectSetMixin<FsFile>::unique;

    using ManagerObjectMixin<CxxIncludeDirectory>::unique;
    using ManagerObjectMixin<CxxIncludeDirectory>::isUnique;

    using ManagerObjectSetMixin<CxxIncludeDirectory>::unique;
    using ManagerObjectSetMixin<CxxIncludeDirectory>::isUnique;

    using ManagerObjectMixin<CxxHeader>::unique;
    using ManagerObjectMixin<CxxHeader>::isUnique;

    using ManagerObjectSetMixin<CxxHeader>::unique;
    using ManagerObjectSetMixin<CxxHeader>::isUnique;

    using ManagerObjectMixin<CxxFile>::unique;
    using ManagerObjectMixin<CxxFile>::isUnique;

    using ManagerObjectMixin<CxxObjectFile>::unique;
    using ManagerObjectMixin<CxxObjectFile>::isUnique;

    using ManagerObjectSetMixin<CxxObjectFile>::unique;
    using ManagerObjectSetMixin<CxxObjectFile>::isUnique;

    using ManagerObjectMixin<CxxStaticLibrary>::unique;
    using ManagerObjectMixin<CxxStaticLibrary>::isUnique;

    using ManagerObjectSetMixin<CxxStaticLibrary>::unique;
    using ManagerObjectSetMixin<CxxStaticLibrary>::isUnique;

    using ManagerObjectMixin<CxxProgram>::unique;
    using ManagerObjectMixin<CxxProgram>::isUnique;

    using ManagerObjectMixin<DbKey>::unique;
    using ManagerObjectMixin<DbKey>::isUnique;

    using ManagerObjectMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::isUnique;

    SysArgumentSPtr obtainArgument(const std::string& value)
    {
        return unique(std::make_shared<doim::SysArgument>(value));
    }

    SysArgumentSetSPtr obtainArguments(const SysArgumentSPtr argument)
    {
        auto arguments = std::make_shared<SysArgumentSet>();
        arguments->insert(argument);
        return unique(arguments);
    }

    using ManagerObjectMixin<SysCommand>::unique;
    using ManagerObjectMixin<SysCommand>::isUnique;

    using ManagerObjectMixin<SysExecutable>::unique;
    using ManagerObjectMixin<SysExecutable>::isUnique;

    using ManagerObjectMixin<Tag>::unique;
    using ManagerObjectMixin<Tag>::find;
    using ManagerObjectSetMixin<Tag>::unique;

private:
    std::unordered_map<FsFileSPtr, CxxHeaderSPtr> mFile2CxxHeader;
};

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory);
std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories);
std::ostream& operator<<(std::ostream& out, const CxxHeader& header);
}
