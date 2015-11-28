//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"
#include "dom/fs/fs_file.hpp"

#include "im/initialization_manager.hpp"

#include <stddef.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>

namespace dom
{
class FsManager
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() + im::InitializationManager::rank_step();
    }

    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base, const std::string& directory)
    {
        return obtainDirectory(base, directory.begin(), directory.end());
    }

    FsDirectorySPtr obtainDirectory(const FsDirectorySPtr& base,
                                    const std::string::const_iterator& begin,
                                    const std::string::const_iterator& end);
    FsFileSPtr obtainFile(const FsDirectorySPtr& base,
                          const std::string::const_iterator& begin,
                          const std::string::const_iterator& end);

private:
    struct Hasher
    {
        std::size_t operator()(const FsDirectorySPtr& directory) const
        {
            return std::hash<const FsDirectory*>()(directory->parent().get()) ^
                   std::hash<std::string>()(directory->name());
        }

        bool operator()(const FsDirectorySPtr& directory1, const FsDirectorySPtr& directory2) const
        {
            return directory1->parent() == directory2->parent() &&
                   directory1->name() == directory2->name();
        }
    };

    std::unordered_set<FsDirectorySPtr, Hasher, Hasher> mDirectories;
};

typedef std::shared_ptr<FsManager> FsManagerSPtr;

extern FsManagerSPtr gFsManager;
}
