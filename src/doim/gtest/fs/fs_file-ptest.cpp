//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"

#include <gtest/gtest.h>

#include <iosfwd>
#include <memory>

TEST(FsFileTest, SLOW_construct)
{
    doim::FsDirectorySPtr directory;

    for (int i = 0; i < 100000; ++i)
        std::make_shared<doim::FsFile>(directory, "this is a file name");
}
