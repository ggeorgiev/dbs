//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_relative_file.h"
#include "err/err_assert.h"
#include "const/constants.h"

namespace doim
{
FsRelativeFile::FsRelativeFile(const FsRelativeDirectorySPtr& directory,
                               const string& name)
    : TreeLeaf(directory, name)
{
    ASSERT(directory->isUnique());
}
}
