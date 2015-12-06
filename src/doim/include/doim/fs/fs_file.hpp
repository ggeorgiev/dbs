//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.hpp"

#include <memory>
#include <string>
#include <utility>

namespace doim
{
class FsFile
{
public:
    FsFile(const FsDirectorySPtr& directory, const std::string& name)
        : mDirectory(directory)
        , mName(name)
    {
    }

    std::string path(const FsDirectorySPtr& directory)
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

private:
    FsDirectorySPtr mDirectory;
    std::string mName;
};

typedef std::shared_ptr<FsFile> FsFileSPtr;
}
