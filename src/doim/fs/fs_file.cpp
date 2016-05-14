//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
FsFile::FsFile(const FsDirectorySPtr& directory, const string& name)
    : Base(directory, name)
{
    ASSERT(gManager->isUnique(directory));
}
}
