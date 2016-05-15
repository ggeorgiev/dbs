//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/fs/fs_directory.h"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class FsFile;
typedef shared_ptr<FsFile> FsFileSPtr;
typedef Set<FsFile> FsFileSet;
typedef shared_ptr<FsFileSet> FsFileSetSPtr;

class FsFile : public Element<FsFile, FsDirectorySPtr, string>
{
public:
    using Element<FsFile, FsDirectorySPtr, string>::find;
    static FsFileSPtr find(const FsDirectorySPtr& base, const string_view& file);

    static FsFileSPtr obtain(const FsDirectorySPtr& base, const string_view& file);

    FsFile(const FsDirectorySPtr& directory, const string& name);

    const FsDirectorySPtr& directory() const
    {
        return std::get<0>(mArgs);
    }

    const string& name() const
    {
        return std::get<1>(mArgs);
    }

    string path(const FsDirectorySPtr& root = nullptr) const
    {
        return directory()->path(root) + name();
    }
};
}
