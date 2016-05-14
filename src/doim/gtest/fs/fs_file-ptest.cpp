//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_file.h"
#include "doim/fs/fs_directory.h"
#include "gtest/framework.h"
#include <memory>

TEST(FsFileTest, VERYSLOW_construct)
{
    doim::FsDirectorySPtr directory;

    for (int i = 0; i < 10000; ++i)
        doim::FsFile::make(directory, "this is a file name");
}
