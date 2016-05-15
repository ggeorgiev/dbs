//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered>

namespace doim
{
class CxxHeader;
typedef shared_ptr<CxxHeader> CxxHeaderSPtr;
typedef NewObjectSet<CxxHeader> CxxHeaderSet;
typedef shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxIncludeDirectory;
typedef shared_ptr<CxxIncludeDirectory> CxxIncludeDirectorySPtr;
typedef NewObjectSet<CxxIncludeDirectory> CxxIncludeDirectorySet;
typedef shared_ptr<CxxIncludeDirectorySet> CxxIncludeDirectorySetSPtr;

struct CxxIncludeDirectoryEnums
{
    enum class EType
    {
        kUser,
        kSystem,
    };
};

class CxxIncludeDirectory : public CxxIncludeDirectoryEnums,
                            public Base<CxxIncludeDirectory,
                                        CxxIncludeDirectoryEnums::EType,
                                        FsDirectorySPtr,
                                        CxxHeaderSetSPtr>
{
public:
    CxxIncludeDirectory(const EType type,
                        const FsDirectorySPtr& directory,
                        const CxxHeaderSetSPtr& headerFiles);

    EType type() const
    {
        return std::get<0>(mArgs);
    }

    const FsDirectorySPtr& directory() const
    {
        return std::get<1>(mArgs);
    }

    const CxxHeaderSetSPtr& headerFiles() const
    {
        return std::get<2>(mArgs);
    }

    void finally();

    CxxHeaderSPtr header(const FsFileSPtr& file) const;

private:
    unordered_map<FsFileSPtr, CxxHeaderSPtr> mFiles;
};

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory);
std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories);
}
