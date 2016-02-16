//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_program.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxProgram::CxxProgram(const FsFileSPtr& file, const CxxObjectFileSetSPtr& cxxObjectFiles)
    : Base(file, cxxObjectFiles)
{
    ASSERT(gManager->isUnique(file));
    ASSERT(gManager->isUnique(cxxObjectFiles));
}
}
