//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_file.h"
#include "err/err_assert.h"

namespace doim
{
CxxFile::CxxFile(const FsFileSPtr& file,
                 const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
    : Element(file, cxxIncludeDirectories)
{
    ASSERT(file->isUnique());
    ASSERT(cxxIncludeDirectories->isUnique());
}
}
