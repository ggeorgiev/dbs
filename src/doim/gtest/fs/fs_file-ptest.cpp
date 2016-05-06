//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "gtest/framework.h"
#include <memory>

#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"

TEST(FsFileTest, VERYSLOW_construct)
{
    doim::FsDirectorySPtr directory;

    for (int i = 0; i < 10000; ++i)
        std::make_shared<doim::FsFile>(directory, "this is a file name");
}
