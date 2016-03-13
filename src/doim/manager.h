//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h" // IWYU pragma: keep
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h" // IWYU pragma: keep
#include "doim/cxx/cxx_program.h"     // IWYU pragma: keep
#include "doim/db/db_key.hpp"         // IWYU pragma: keep
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/location.hpp"
#include "doim/generic/object.h"
#include "doim/manager_object_mixin.hpp"
#include "doim/manager_object_set_mixin.hpp"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.h" // IWYU pragma: keep
#include "doim/sys/executable.hpp"
#include "im/initialization_manager.hpp"
#include <experimental/string_view>
#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace doim
{
class Manager : public ManagerObjectMixin<CxxFile>,
                public ManagerObjectSetMixin<CxxHeader>,
                public ManagerObjectSetMixin<CxxIncludeDirectory>,
                public ManagerObjectSetMixin<CxxObjectFile>,
                public ManagerObjectMixin<CxxProgram>,
                public ManagerObjectMixin<DbKey>,
                public ManagerObjectMixin<FsDirectory>,
                public ManagerObjectSetMixin<FsFile>,
                public ManagerObjectMixin<Object>,
                public ManagerObjectSetMixin<SysArgument>,
                public ManagerObjectMixin<SysCommand>
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    // Obtain an unique location.
    LocationSPtr obtainLocation(const LocationSPtr& base,
                                const std::experimental::string_view& location)
    {
        return obtainDirectory(base, location);
    }

    using ManagerObjectMixin<Object>::unique;

    // Obtain an unique object.
    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::experimental::string_view& object);

    using ManagerObjectMixin<FsDirectory>::unique;
    using ManagerObjectMixin<FsDirectory>::isUnique;

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

    FsFileSPtr createFile(const FsDirectorySPtr& base,
                          const std::experimental::string_view& file);

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::experimental::string_view& file)
    {
        return unique(createFile(base, file));
    }

    using ManagerObjectSetMixin<FsFile>::unique;

    using ManagerObjectMixin<CxxIncludeDirectory>::unique;
    using ManagerObjectMixin<CxxIncludeDirectory>::isUnique;

    using ManagerObjectSetMixin<CxxIncludeDirectory>::unique;
    using ManagerObjectSetMixin<CxxIncludeDirectory>::isUnique;

    using ManagerObjectMixin<CxxHeader>::isUnique;

    CxxHeaderSPtr unique(const CxxHeaderSPtr& header)
    {
        if (header == nullptr)
            return header;
        const auto& record = ManagerObjectMixin<CxxHeader>::mMixinObjects.insert(header);
        if (record.second)
            mFile2CxxHeader[header->file()] = header;
        return *record.first;
    }

    CxxHeaderSPtr findCxxHeader(const FsFileSPtr& file)
    {
        return mFile2CxxHeader[file];
    }

    using ManagerObjectSetMixin<CxxHeader>::unique;
    using ManagerObjectSetMixin<CxxHeader>::isUnique;

    using ManagerObjectMixin<CxxFile>::unique;
    using ManagerObjectMixin<CxxFile>::isUnique;

    using ManagerObjectMixin<CxxObjectFile>::unique;
    using ManagerObjectMixin<CxxObjectFile>::isUnique;

    using ManagerObjectSetMixin<CxxObjectFile>::unique;
    using ManagerObjectSetMixin<CxxObjectFile>::isUnique;

    using ManagerObjectMixin<CxxProgram>::unique;
    using ManagerObjectMixin<CxxProgram>::isUnique;

    using ManagerObjectMixin<DbKey>::unique;
    using ManagerObjectMixin<DbKey>::isUnique;

    using ManagerObjectMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::unique;
    using ManagerObjectSetMixin<SysArgument>::isUnique;

    SysExecutableSPtr obtainExecutable(const FsDirectorySPtr& base,
                                       const std::experimental::string_view& file)
    {
        return obtainFile(base, file);
    }

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

private:
    std::unordered_map<FsFileSPtr, CxxHeaderSPtr> mFile2CxxHeader;
};

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory);
std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories);
std::ostream& operator<<(std::ostream& out, const CxxHeader& header);

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
