//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxFile::CxxFile(const FsFileSPtr& file,
                 const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
    : Base(file, cxxIncludeDirectories)
{
    ASSERT(gManager->isUnique(file));
    ASSERT(gManager->isUnique(cxxIncludeDirectories));
}
}
