//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include <gtest/gtest.h>
#include <unordered_set>

TEST(CxxProgramTest, foo)
{
    dom::CxxProgram cxxProgram;

    std::unordered_set<doim::FsFileSPtr> files;
    cxxProgram.updateCxxFilesList(files);
}
