//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_file.h"
#include "err/err_assert.h"

namespace doim
{
CxxFile::CxxFile(const FsFileSPtr& file,
                 const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories,
                 const OriginSPtr& origin)
    : Element(file, cxxIncludeDirectories, origin)
{
    ASSERT(file->isUnique());
    ASSERT(cxxIncludeDirectories->isUnique());
    ASSERT(boost::apply_visitor(
        [](auto const& origin) { return origin == nullptr || origin->isUnique(); },
        origin));
}
}
