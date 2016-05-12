//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_object_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxObjectFile::CxxObjectFile(EPurpose purpose,
                             const CxxFileSPtr& cxxFile,
                             const FsFileSPtr& file)
    : Base(static_cast<typename std::underlying_type<EPurpose>::type>(purpose),
           cxxFile,
           file)
{
    ASSERT(gManager->isUnique(cxxFile));
    ASSERT(gManager->isUnique(file));
}
}
