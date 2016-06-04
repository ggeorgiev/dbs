//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_relative_directory.h"
#include "err/err_assert.h"
#include "const/constants.h"
#include <algorithm>

namespace doim
{
FsRelativeDirectory::FsRelativeDirectory()
    : TreeNode(FsRelativeDirectorySPtr(), string())
{
}

FsRelativeDirectory::FsRelativeDirectory(const FsRelativeDirectorySPtr& parent,
                                         const string& name)
    : TreeNode(parent, name)
{
    ASSERT(parent->isUnique());
}
}
