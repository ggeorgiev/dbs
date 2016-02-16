//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxFile;

typedef std::shared_ptr<CxxFile> CxxFileSPtr;
typedef std::unordered_set<CxxFileSPtr> CxxFileSet;
typedef std::shared_ptr<CxxFileSet> CxxFileSetSPtr;

class CxxFile : public Base<CxxFile, FsFileSPtr, CxxIncludeDirectorySetSPtr>
{
public:
    CxxFile(const FsFileSPtr& file,
            const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories);

    const FsFileSPtr& file() const
    {
        return std::get<0>(mArgs);
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return std::get<1>(mArgs);
    }
};
}
