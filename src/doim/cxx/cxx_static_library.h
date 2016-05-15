//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <memory>
#include <tuple>
#include <unordered>

namespace doim
{
class CxxStaticLibrary;
typedef shared_ptr<CxxStaticLibrary> CxxStaticLibrarySPtr;
typedef NewObjectSet<CxxStaticLibrary> CxxStaticLibrarySet;
typedef shared_ptr<CxxStaticLibrarySet> CxxStaticLibrarySetSPtr;

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
