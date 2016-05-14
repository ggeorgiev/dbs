//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include "gtest/framework.h"
#include <unordered_set>

TEST(CxxProgramTest, foo)
{
    dom::CxxProgram cxxProgram;

    doim::FsFileSet files;
    cxxProgram.updateCxxFilesList(files);
}
