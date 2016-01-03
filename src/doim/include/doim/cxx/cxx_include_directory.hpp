//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class CxxIncludeDirectory;
typedef std::shared_ptr<CxxIncludeDirectory> CxxIncludeDirectorySPtr;

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
                        const FsFileSetSPtr& headerFiles)
        : mType(type)
        , mDirectory(directory)
        , mHeaderFiles(headerFiles)
    {
    }

    Type& type()
    {
        return mType;
    }

    const FsDirectorySPtr& directory()
    {
        return mDirectory;
    }

    const FsFileSetSPtr& headerFiles()
    {
        return mHeaderFiles;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxIncludeDirectorySPtr& directory) const
        {
            return std::hash<int>()(static_cast<int>(directory->type())) ^
                   std::hash<FsDirectorySPtr>()(directory->directory()) ^
                   std::hash<FsFileSetSPtr>()(directory->headerFiles());
        }

        bool operator()(const CxxIncludeDirectorySPtr& directory1,
                        const CxxIncludeDirectorySPtr& directory2) const
        {
            return directory1->type() == directory2->type() &&
                   directory1->directory() == directory2->directory() &&
                   directory1->headerFiles() == directory2->headerFiles();
        }
    };

private:
    Type mType;
    FsDirectorySPtr mDirectory;
    FsFileSetSPtr mHeaderFiles;
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
