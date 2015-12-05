//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"

#include <memory>
#include <string>
#include <utility>

namespace dom
{
class FsFile
{
public:
    std::string path(const FsDirectorySPtr& directory)
    {
        return mDirectory->path(directory) + mName;
    }

    const std::string& name() const { return mName; }
    void set_name(const std::string& name) { mName = name; }
    // void set_name(std::string&& name) { mName = std::forward<std::string>(name); }
    const FsDirectorySPtr& directory() const { return mDirectory; }
    void set_directory(const FsDirectorySPtr& directory) { mDirectory = directory; }
private:
    FsDirectorySPtr mDirectory;
    std::string mName;
};

typedef std::shared_ptr<FsFile> FsFileSPtr;
}
