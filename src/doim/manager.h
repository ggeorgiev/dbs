//  Copyright © 2015-1016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h" // IWYU pragma: keep
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_program.h" // IWYU pragma: keep
#include "doim/db/db_key.hpp"     // IWYU pragma: keep
#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/generic/location.hpp"
#include "doim/generic/object.hpp"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.hpp" // IWYU pragma: keep
#include "doim/sys/executable.hpp"
#include "im/initialization_manager.hpp"
#include <boost/functional/hash/hash.hpp>
#include <algorithm>
#include <experimental/string_view>
#include <iosfwd>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <stddef.h>

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

    bool isUnique(const MixinObjectSPtr& object) const
    {
        return object == find(object);
    }

    MixinObjectSPtr find(const MixinObjectSPtr& object) const
    {
        if (object == nullptr)
            return object;
        const auto& it = mMixinObjects.find(object);
        if (it == mMixinObjects.end())
            return nullptr;
        return *it;
    }

protected:
    std::unordered_set<MixinObjectSPtr, Hasher, Hasher> mMixinObjects;
};

template <typename T>
struct SetHasher
{
    typedef std::shared_ptr<T> ObjectSPtr;
    typedef std::unordered_set<ObjectSPtr> ObjectSet;
    typedef std::shared_ptr<ObjectSet> ObjectSetSPtr;

    std::size_t operator()(const ObjectSetSPtr& objects) const
    {
        std::vector<ObjectSPtr> vector(objects->size());
        vector.insert(vector.begin(), objects->begin(), objects->end());
        std::sort(vector.begin(), vector.end());

        std::hash<ObjectSPtr> hash;

        std::size_t seed = 0;
        for (const auto& object : vector)
            boost::hash_combine(seed, hash(object));

        return seed;
    }

    bool operator()(const ObjectSetSPtr& objects1, const ObjectSetSPtr& objects2) const
    {
        return *objects1 == *objects2;
    }
};

typedef SetHasher<CxxHeader> CxxHeaderSetHasher;
typedef SetHasher<CxxIncludeDirectory> CxxIncludeDirectorySetHasher;
typedef SetHasher<CxxObjectFile> CxxObjectFileSetHasher;

class Manager : public ManagerMixin<CxxFile>,
                public ManagerMixin<CxxHeader>,
                public ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>,
                public ManagerMixin<CxxIncludeDirectory>,
                public ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>,
                public ManagerMixin<CxxObjectFile>,
                public ManagerMixin<CxxObjectFileSet, CxxObjectFileSetHasher>,
                public ManagerMixin<CxxProgram>,
                public ManagerMixin<DbKey>,
                public ManagerMixin<FsDirectory>,
                public ManagerMixin<FsFile>,
                public ManagerMixin<FsFileSet, FsFileSetHasher>,
                public ManagerMixin<Object>,
                public ManagerMixin<SysArgument>,
                public ManagerMixin<SysArgumentSet, SysArgumentSetHasher>,
                public ManagerMixin<SysCommand>
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
    using ManagerMixin<FsDirectory>::isUnique;

    // Obtain an unique directory.
    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::experimental::string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    FsDirectorySPtr obtainCorrespondingDirectory(const FsDirectorySPtr& directory,
                                                 const FsDirectorySPtr& fromDirectory,
                                                 const FsDirectorySPtr& toDirectory);

    using ManagerMixin<FsFile>::unique;
    using ManagerMixin<FsFile>::isUnique;
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
    using ManagerMixin<CxxIncludeDirectory>::isUnique;

    using ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>::unique;
    using ManagerMixin<CxxIncludeDirectorySet, CxxIncludeDirectorySetHasher>::isUnique;

    CxxHeaderSPtr unique(const CxxHeaderSPtr& header)
    {
        if (header == nullptr)
            return header;
        const auto& record = ManagerMixin<CxxHeader>::mMixinObjects.insert(header);
        if (record.second)
            mFile2CxxHeader[header->file()] = header;
        return *record.first;
    }

    CxxHeaderSPtr findCxxHeader(const FsFileSPtr& file)
    {
        return mFile2CxxHeader[file];
    }

    using ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>::unique;
    using ManagerMixin<CxxHeaderSet, CxxHeaderSetHasher>::isUnique;

    using ManagerMixin<CxxFile>::unique;
    using ManagerMixin<CxxFile>::isUnique;

    using ManagerMixin<CxxObjectFile>::unique;
    using ManagerMixin<CxxObjectFile>::isUnique;

    using ManagerMixin<CxxObjectFileSet, CxxObjectFileSetHasher>::unique;
    using ManagerMixin<CxxObjectFileSet, CxxObjectFileSetHasher>::isUnique;

    using ManagerMixin<CxxProgram>::unique;

    using ManagerMixin<DbKey>::unique;

    using ManagerMixin<SysArgument>::unique;
    using ManagerMixin<SysArgumentSet, SysArgumentSetHasher>::unique;

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

    using ManagerMixin<SysCommand>::unique;

private:
    std::unordered_map<FsFileSPtr, CxxHeaderSPtr> mFile2CxxHeader;
};

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory);
std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories);
std::ostream& operator<<(std::ostream& out, const CxxHeader& header);

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
