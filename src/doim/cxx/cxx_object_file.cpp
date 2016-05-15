//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_object_file.h"

#include "err/err_assert.h"

namespace doim
{
CxxObjectFile::CxxObjectFile(EPurpose purpose,
                             const CxxFileSPtr& cxxFile,
                             const FsFileSPtr& file)
    : Element(purpose, cxxFile, file)
{
    ASSERT(cxxFile->isUnique());
    ASSERT(file->isUnique());
}
}
