//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.hpp" // IWYU pragma: keep
#include "doim/cxx/cxx_header.hpp"
#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/cxx/cxx_object_file.hpp" // IWYU pragma: keep
#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/generic/location.hpp"
#include "doim/generic/object.hpp"
#include "im/initialization_manager.hpp"
#include <experimental/string_view>
#include <memory>
#include <unordered_set>

namespace doim
{
template <typename T, typename H = typename T::Hasher>
class ManagerMixin
{
public:
    typedef T MixinObject;
    typedef std::shared_ptr<MixinObject> MixinObjectSPtr;

    typedef H Hasher;

    MixinObjectSPtr unique(const MixinObjectSPtr& object)
    {
        if (object == nullptr)
            return object;
        return *mMixinObjects.insert(object).first;
    }

    MixinObjectSPtr find(const MixinObjectSPtr& object) const
    {
        if (object == nullptr)
            return object;
        auto it = mMixinObjects.find(object);
        if (it == mMixinObjects.end())
            return nullptr;
        return *it;
    }

protected:
    std::unordered_set<MixinObjectSPtr, Hasher, Hasher> mMixinObjects;
};

class Manager : public ManagerMixin<Object>,
                public ManagerMixin<FsDirectory>,
                public ManagerMixin<FsFile>,
                public ManagerMixin<FsFileSet, FsFileSetHasher>,
                public ManagerMixin<CxxIncludeDirectory>,
                public ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>,
                public ManagerMixin<CxxHeader>,
                public ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>,
                public ManagerMixin<CxxFile>,
                public ManagerMixin<CxxObjectFile>
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

    using ManagerMixin<Object>::unique;

    // Obtain an unique object.
    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::experimental::string_view& object);

    using ManagerMixin<FsDirectory>::unique;

    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::experimental::string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    FsDirectorySPtr obtainCorrespondingDirectory(const FsDirectorySPtr& directory,
                                                 const FsDirectorySPtr& fromDirectory,
                                                 const FsDirectorySPtr& toDirectory);

    using ManagerMixin<FsFile>::unique;
    using ManagerMixin<FsFile>::find;

    FsFileSPtr createFile(const FsDirectorySPtr& base,
                          const std::experimental::string_view& file);

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::experimental::string_view& file)
    {
        return unique(createFile(base, file));
    }

    using ManagerMixin<FsFileSet, FsFileSetHasher>::unique;

    using ManagerMixin<CxxIncludeDirectory>::unique;

    using ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>::unique;

    using ManagerMixin<CxxHeader>::unique;
    using ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>::unique;

    using ManagerMixin<CxxFile>::unique;
    using ManagerMixin<CxxObjectFile>::unique;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
