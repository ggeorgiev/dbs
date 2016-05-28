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
    ASSERT(boost::get<CxxFileSPtr>(source)->isUnique());
    ASSERT(file->isUnique());
}
}
