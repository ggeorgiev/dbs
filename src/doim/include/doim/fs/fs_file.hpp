//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class FsFile;
typedef std::shared_ptr<FsFile> FsFileSPtr;

class FsFile
{
public:
    FsFile(const FsDirectorySPtr& directory, const std::string& name)
        : mDirectory(directory)
        , mName(name)
    {
    }

    std::string path(const FsDirectorySPtr& directory = nullptr)
    {
        return mDirectory->path(directory) + mName;
    }

    const std::string& name() const
    {
        return mName;
    }

    const FsDirectorySPtr& directory() const
    {
        return mDirectory;
    }

    struct Hasher
    {
        std::size_t operator()(const FsFileSPtr& file) const
        {
            return std::hash<FsDirectorySPtr>()(file->directory()) ^
                   std::hash<std::string>()(file->name());
        }

        bool operator()(const FsFileSPtr& file1, const FsFileSPtr& file2) const
        {
            return file1->directory() == file2->directory() &&
                   file1->name() == file2->name();
        }
    };

private:
    FsDirectorySPtr mDirectory;
    std::string mName;
};

typedef std::unordered_set<FsFileSPtr> FsFileSet;
typedef std::shared_ptr<FsFileSet> FsFileSetSPtr;

struct FsFileSetHasher
{
    std::size_t operator()(const FsFileSetSPtr& files) const
    {
        std::vector<FsFileSPtr> vector(files->size());
        vector.insert(vector.begin(), files->begin(), files->end());
        std::sort(vector.begin(), vector.end());

        std::size_t hash = 0;
        for (const auto& file : vector)
            hash ^ std::hash<FsFileSPtr>()(file);

        return hash;
    }

    bool operator()(const FsFileSetSPtr& files1, const FsFileSetSPtr& files2) const
    {
        return *files1 == *files2;
    }
};
}
