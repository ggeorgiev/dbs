//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_program.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxProgram::CxxProgram(EPurpose purpose,
                       const FsFileSPtr& file,
                       const CxxStaticLibrarySetSPtr& staticLibraries,
                       const CxxObjectFileSetSPtr& cxxObjectFiles)
    : Base(purpose, file, staticLibraries, cxxObjectFiles)
{
    ASSERT(gManager->isUnique(file));
    ASSERT(gManager->isUnique(staticLibraries));
    ASSERT(cxxObjectFiles->isUnique());
}
}
