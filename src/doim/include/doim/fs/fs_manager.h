//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"

#include "im/initialization_manager.hpp"

#include <stddef.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>

namespace doim
{
class FsManager
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::string& directory)
    {
        return obtainDirectory(base, directory.begin(), directory.end());
    }

    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::string::const_iterator& begin,
                                    const std::string::const_iterator& end);
    FsFileSPtr obtainFile(const FsDirectorySPtr& base, const std::string& file)
    {
        return obtainFile(base, file.begin(), file.end());
    }

    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::string::const_iterator& begin,
                          const std::string::const_iterator& end);

private:
    struct DirectoryHasher
    {
        std::size_t operator()(const FsDirectorySPtr& directory) const
        {
            return std::hash<FsDirectorySPtr>()(directory->parent()) ^
                   std::hash<std::string>()(directory->name());
        }

        bool operator()(const FsDirectorySPtr& directory1,
                        const FsDirectorySPtr& directory2) const
        {
            return directory1->parent() == directory2->parent() &&
                   directory1->name() == directory2->name();
        }
    };
    std::unordered_set<FsDirectorySPtr, DirectoryHasher, DirectoryHasher> mDirectories;

    struct FileHasher
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

    std::unordered_set<FsFileSPtr, FileHasher, FileHasher> mFiles;
};

typedef std::shared_ptr<FsManager> FsManagerSPtr;

extern FsManagerSPtr gFsManager;
}
