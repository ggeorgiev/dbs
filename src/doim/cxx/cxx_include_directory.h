//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered>

namespace doim
{
class CxxHeader;

typedef shared_ptr<CxxHeader> CxxHeaderSPtr;
typedef unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxIncludeDirectory;

typedef shared_ptr<CxxIncludeDirectory> CxxIncludeDirectorySPtr;
typedef unordered_set<CxxIncludeDirectorySPtr> CxxIncludeDirectorySet;
typedef shared_ptr<CxxIncludeDirectorySet> CxxIncludeDirectorySetSPtr;

namespace cid_details
{
enum class EType
{
    kUser,
    kSystem,
};
}

class CxxIncludeDirectory : public Base<CxxIncludeDirectory,
                                        std::underlying_type<cid_details::EType>::type,
                                        FsDirectorySPtr,
                                        CxxHeaderSetSPtr>
{
public:
    typedef cid_details::EType EType;

    CxxIncludeDirectory(const EType type,
                        const FsDirectorySPtr& directory,
                        const CxxHeaderSetSPtr& headerFiles);

    EType type() const
    {
        return static_cast<EType>(std::get<0>(mArgs));
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
}
