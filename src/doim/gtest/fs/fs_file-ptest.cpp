//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include <gtest/gtest.h>
#include <memory>

#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"

TEST(FsFileTest, SLOW_construct)
{
    doim::FsDirectorySPtr directory;

    for (int i = 0; i < 100000; ++i)
        std::make_shared<doim::FsFile>(directory, "this is a file name");
}
