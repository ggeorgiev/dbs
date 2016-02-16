//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_directory.hpp"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxHeader;

typedef std::shared_ptr<CxxHeader> CxxHeaderSPtr;
typedef std::unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef std::shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxIncludeDirectory;

typedef std::shared_ptr<CxxIncludeDirectory> CxxIncludeDirectorySPtr;
typedef std::unordered_set<CxxIncludeDirectorySPtr> CxxIncludeDirectorySet;
typedef std::shared_ptr<CxxIncludeDirectorySet> CxxIncludeDirectorySetSPtr;

class CxxIncludeDirectory
    : public Base<CxxIncludeDirectory, int, FsDirectorySPtr, CxxHeaderSetSPtr>
{
public:
    enum class Type
    {
        kUser,
        kSystem,
    };

    CxxIncludeDirectory(const Type type,
                        const FsDirectorySPtr& directory,
                        const CxxHeaderSetSPtr& headerFiles);

    Type type() const
    {
        return static_cast<Type>(std::get<0>(mArgs));
    }

    const FsDirectorySPtr& directory() const
    {
        return std::get<1>(mArgs);
    }

    const CxxHeaderSetSPtr& headerFiles() const
    {
        return std::get<2>(mArgs);
    }
};
}
