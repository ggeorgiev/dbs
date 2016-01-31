//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "err/err_assert.h"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class CxxIncludeDirectory;
typedef std::shared_ptr<CxxIncludeDirectory> CxxIncludeDirectorySPtr;

class CxxHeader;
typedef std::shared_ptr<CxxHeader> CxxHeaderSPtr;

typedef std::unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef std::shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxIncludeDirectory
{
public:
    enum class Type
    {
        kUser,
        kSystem,
    };

    CxxIncludeDirectory(const Type type,
                        const FsDirectorySPtr& directory,
                        const CxxHeaderSetSPtr& headerFiles)
        : mType(type)
        , mDirectory(directory)
        , mHeaderFiles(headerFiles)
    {
        ASSERT(mDirectory != nullptr);
    }

    Type type()
    {
        return mType;
    }

    const FsDirectorySPtr& directory()
    {
        return mDirectory;
    }

    const CxxHeaderSetSPtr& headerFiles()
    {
        return mHeaderFiles;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxIncludeDirectorySPtr& directory) const
        {
            auto seed = std::hash<int>()(static_cast<int>(directory->type()));
            boost::hash_combine(seed, hashFD(directory->directory()));
            boost::hash_combine(seed, hashFFS(directory->headerFiles()));
            return seed;
        }

        bool operator()(const CxxIncludeDirectorySPtr& directory1,
                        const CxxIncludeDirectorySPtr& directory2) const
        {
            return directory1->type() == directory2->type() &&
                   directory1->directory() == directory2->directory() &&
                   directory1->headerFiles() == directory2->headerFiles();
        }

        std::hash<FsDirectorySPtr> hashFD;
        std::hash<CxxHeaderSetSPtr> hashFFS;
    };

private:
    Type mType;
    FsDirectorySPtr mDirectory;
    CxxHeaderSetSPtr mHeaderFiles;
};

typedef std::unordered_set<CxxIncludeDirectorySPtr> CxxIncludeDirectorySet;
typedef std::shared_ptr<CxxIncludeDirectorySet> CxxIncludeDirectorySetSPtr;

struct CxxIncludeDirectorySetHasher
{
    std::size_t operator()(const CxxIncludeDirectorySetSPtr& directories) const
    {
        std::vector<CxxIncludeDirectorySPtr> vector(directories->size());
        vector.insert(vector.begin(), directories->begin(), directories->end());
        std::sort(vector.begin(), vector.end());

        std::size_t hash = 0;
        for (const auto& directory : vector)
            hash ^ std::hash<CxxIncludeDirectorySPtr>()(directory);

        return hash;
    }

    bool operator()(const CxxIncludeDirectorySetSPtr& directories1,
                    const CxxIncludeDirectorySetSPtr& directories2) const
    {
        return *directories1 == *directories2;
    }
};
}
