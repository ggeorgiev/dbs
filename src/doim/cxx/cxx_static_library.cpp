//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_static_library.h"
#include "err/err_assert.h"

namespace doim
{
CxxStaticLibrary::CxxStaticLibrary(const FsFileSPtr& binary)
    : Element(binary)
{
    ASSERT(binary->isUnique());
}
}
