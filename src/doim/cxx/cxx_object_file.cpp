//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_object_file.h"
#include "err/err_assert.h"

namespace doim
{
CxxObjectFile::CxxObjectFile(EPurpose purpose,
                             const SourceSPtr& source,
                             const FsFileSPtr& file)
    : Element(purpose, source, file)
{
    ASSERT(boost::apply_visitor(
        [](auto const& source) { return source != nullptr && source->isUnique(); },
        source));
    ASSERT(file->isUnique());
}
}
