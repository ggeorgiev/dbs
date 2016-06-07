//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_program.h"
#include "err/err_assert.h"

namespace doim
{
CxxProgram::CxxProgram(EPurpose purpose,
                       const FsFileSPtr& file,
                       const CxxStaticLibrarySetSPtr& staticLibraries,
                       const CxxObjectFileSetSPtr& cxxObjectFiles)
    : Element(purpose, file, staticLibraries, cxxObjectFiles)
{
    ASSERT(file->isUnique());
    ASSERT(staticLibraries->isUnique());
    ASSERT(cxxObjectFiles->isUnique());
}
}
