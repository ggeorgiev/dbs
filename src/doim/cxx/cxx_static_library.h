//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxStaticLibrary;
typedef std::shared_ptr<CxxStaticLibrary> CxxStaticLibrarySPtr;
typedef SetBase<CxxStaticLibrarySPtr> CxxStaticLibrarySet;
typedef std::shared_ptr<CxxStaticLibrarySet> CxxStaticLibrarySetSPtr;

class CxxStaticLibrary : public Base<CxxStaticLibrary, FsFileSPtr>
{
public:
    CxxStaticLibrary(const FsFileSPtr& binary);

    const FsFileSPtr& binary() const
    {
        return std::get<0>(mArgs);
    }
};
}
