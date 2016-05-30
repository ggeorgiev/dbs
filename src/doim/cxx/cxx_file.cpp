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
    ASSERT(file != nullptr && file->isUnique());
    ASSERT(cxxIncludeDirectories != nullptr && cxxIncludeDirectories->isUnique());
    ASSERT(apply_visitor(vst::isUniqueIfAny, origin));
}
}
