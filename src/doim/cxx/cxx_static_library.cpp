//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_static_library.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxStaticLibrary::CxxStaticLibrary(const FsFileSPtr& file)
    : Base(file)
{
    ASSERT(gManager->isUnique(file));
}
}
