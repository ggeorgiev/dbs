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
    : Base(static_cast<typename std::underlying_type<EPurpose>::type>(purpose),
           file,
           staticLibraries,
           cxxObjectFiles)
{
    ASSERT(gManager->isUnique(file));
    ASSERT(gManager->isUnique(staticLibraries));
    ASSERT(gManager->isUnique(cxxObjectFiles));
}
}
