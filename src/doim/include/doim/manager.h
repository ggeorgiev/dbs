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
class Manager
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

    FsFileSPtr obtainFile(const FsDirectorySPtr& base, const std::string& file)
    {
        return obtainFile(base, file.begin(), file.end());
    }

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::string::const_iterator& begin,
                          const std::string::const_iterator& end);

    CxxIncludeDirectorySPtr obtainCxxIncludeDirectory(CxxIncludeDirectory::Type type,
                                                      const FsDirectorySPtr& directory)
    {
        return *mCxxIncludeDirectory.insert(
                                        std::make_shared<CxxIncludeDirectory>(type,
                                                                              directory))
                    .first;
    }

    CxxIncludeDirectorySetSPtr unique(const CxxIncludeDirectorySetSPtr& directories)
    {
        return *mCxxIncludeDirectorySets.insert(directories).first;
    }

    CxxHeaderSetSPtr unique(const CxxHeaderSetSPtr& headers)
    {
        return *mCxxHeaderSets.insert(headers).first;
    }

    CxxFileSPtr unique(const CxxFileSPtr& cxxFile)
    {
        return *mCxxFiles.insert(cxxFile).first;
    }

    CxxObjectFileSPtr unique(const CxxObjectFileSPtr& cxxObjectFile)
    {
        return *mCxxObjectFiles.insert(cxxObjectFile).first;
    }

private:
    std::unordered_set<ObjectSPtr, Object::Hasher, Object::Hasher> mObjects;
    std::unordered_set<FsDirectorySPtr, FsDirectory::Hasher, FsDirectory::Hasher>
        mDirectories;
    std::unordered_set<FsFileSPtr, FsFile::Hasher, FsFile::Hasher> mFiles;

    std::unordered_set<CxxIncludeDirectorySPtr,
                       CxxIncludeDirectory::Hasher,
                       CxxIncludeDirectory::Hasher> mCxxIncludeDirectory;
    std::unordered_set<CxxIncludeDirectorySetSPtr,
                       CxxIncludeDirectorySetHasher,
                       CxxIncludeDirectorySetHasher> mCxxIncludeDirectorySets;

    std::unordered_set<CxxHeaderSetSPtr, CxxHeaderSetHasher, CxxHeaderSetHasher>
        mCxxHeaderSets;

    std::unordered_set<CxxFileSPtr, CxxFile::Hasher, CxxFile::Hasher> mCxxFiles;
    std::unordered_set<CxxObjectFileSPtr, CxxObjectFile::Hasher, CxxObjectFile::Hasher>
        mCxxObjectFiles;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
