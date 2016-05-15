//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <memory>
#include <tuple>

namespace doim
{
class CxxStaticLibrary;
typedef shared_ptr<CxxStaticLibrary> CxxStaticLibrarySPtr;
typedef Set<CxxStaticLibrary> CxxStaticLibrarySet;
typedef shared_ptr<CxxStaticLibrarySet> CxxStaticLibrarySetSPtr;

class CxxStaticLibrary : public Element<CxxStaticLibrary, FsFileSPtr>
{
public:
    CxxStaticLibrary(const FsFileSPtr& binary);

    const FsFileSPtr& binary() const
    {
        return std::get<0>(mArgs);
    }
};
}
