//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_directory.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <tuple>
#include <unordered>

namespace doim
{
class FsFile;
typedef std::shared_ptr<FsFile> FsFileSPtr;
typedef unordered_set<FsFileSPtr> FsFileSet;
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
