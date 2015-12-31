//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/cxx/cxx_header.hpp"
#include "doim/cxx/cxx_file.hpp"
#include "doim/cxx/cxx_object_file.hpp"
#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/generic/location.hpp"
#include "doim/generic/object.hpp"

#include "im/initialization_manager.hpp"

#include <__hash_table>
#include <iosfwd>
#include <memory>
#include <unordered_set>
#include <utility>

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
        return *mMixinObjects.insert(object).first;
    }

protected:
    std::unordered_set<MixinObjectSPtr, Hasher, Hasher> mMixinObjects;
};

class Manager : public ManagerMixin<Object>,
                public ManagerMixin<FsDirectory>,
                public ManagerMixin<FsFile>,
                public ManagerMixin<CxxIncludeDirectory>,
                public ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>,
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
    LocationSPtr obtainLocation(const LocationSPtr& base, const std::string& location)
    {
        return obtainDirectory(base, location.begin(), location.end());
    }

    // Obtain an unique location.
    LocationSPtr obtainLocation(const LocationSPtr& base,
                                const std::string::const_iterator& begin,
                                const std::string::const_iterator& end)
    {
        return obtainDirectory(base, begin, end);
    }

    using ManagerMixin<Object>::unique;

    // Obtain an unique object.
    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::string& object)
    {
        return obtainObject(base, type, object.begin(), object.end());
    }

    // Obtain an unique object.
    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::string::const_iterator& begin,
                            const std::string::const_iterator& end);


    using ManagerMixin<FsDirectory>::unique;

    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::string& directory)
    {
        return obtainDirectory(base, directory.begin(), directory.end());
    }

    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::string::const_iterator& begin,
                                    const std::string::const_iterator& end);

    // Obtain an the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    FsDirectorySPtr obtainCorrespondingDirectory(const FsDirectorySPtr& directory,
                                                 const FsDirectorySPtr& fromDirectory,
                                                 const FsDirectorySPtr& toDirectory);

    using ManagerMixin<FsFile>::unique;

    FsFileSPtr obtainFile(const FsDirectorySPtr& base, const std::string& file)
    {
        return obtainFile(base, file.begin(), file.end());
    }

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::string::const_iterator& begin,
                          const std::string::const_iterator& end);

    using ManagerMixin<CxxIncludeDirectory>::unique;

    using ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>::unique;
    using ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>::unique;

    using ManagerMixin<CxxFile>::unique;
    using ManagerMixin<CxxObjectFile>::unique;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
