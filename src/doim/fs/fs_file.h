//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_directory.h"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class FsFile;
typedef std::shared_ptr<FsFile> FsFileSPtr;
typedef std::unordered_set<FsFileSPtr> FsFileSet;
typedef std::shared_ptr<FsFileSet> FsFileSetSPtr;

class FsFile : public Base<FsFile, FsDirectorySPtr, std::string>
{
public:
    FsFile(const FsDirectorySPtr& directory, const std::string& name);

    std::string path(const FsDirectorySPtr& root = nullptr) const
    {
        return directory()->path(root) + name();
    }

    const FsDirectorySPtr& directory() const
    {
        return std::get<0>(mArgs);
    }

    const std::string& name() const
    {
        return std::get<1>(mArgs);
    }
};
}
