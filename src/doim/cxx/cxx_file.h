//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <memory>
#include <tuple>
#include <unordered>

namespace doim
{
class CxxFile;
typedef shared_ptr<CxxFile> CxxFileSPtr;
typedef Set<CxxFile> CxxFileSet;
typedef shared_ptr<CxxFileSet> CxxFileSetSPtr;

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
