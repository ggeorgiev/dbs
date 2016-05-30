//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_file.h"
#include "err/err_assert.h"
#include <variant>

namespace doim
{
CxxFile::CxxFile(const FsFileSPtr& file,
                 const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories,
                 const OriginSPtr& origin)
    : Element(file, cxxIncludeDirectories, origin)
{
    ASSERT(file->isUnique());
    ASSERT(cxxIncludeDirectories->isUnique());
    ASSERT(apply_visitor(vst::isUniqueIfAny, origin));
}
}
