//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

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

    CxxIncludeDirectory(const Type type, const FsDirectorySPtr& directory)
        : mType(type)
        , mDirectory(directory)
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

    struct Hasher
    {
        std::size_t operator()(const CxxIncludeDirectorySPtr& directory) const
        {
            return std::hash<int>()(static_cast<int>(directory->type())) ^
                   std::hash<FsDirectorySPtr>()(directory->directory());
        }

        bool operator()(const CxxIncludeDirectorySPtr& directory1,
                        const CxxIncludeDirectorySPtr& directory2) const
        {
            return directory1->type() == directory2->type() &&
                   directory1->directory() == directory2->directory();
        }
    };

private:
    Type mType;
    FsDirectorySPtr mDirectory;
};

typedef std::unordered_set<CxxIncludeDirectorySPtr> CxxIncludeDirectorySet;
typedef std::shared_ptr<CxxIncludeDirectorySet> CxxIncludeDirectorySetSPtr;
}
