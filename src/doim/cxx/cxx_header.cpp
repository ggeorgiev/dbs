//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_header.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxHeader::CxxHeader(const EType type,
                     const FsFileSPtr& file,
                     const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
    : Base(type, file, cxxIncludeDirectories)
{
    ASSERT(gManager->isUnique(file));
    ASSERT(gManager->isUnique(cxxIncludeDirectories));
}
}
