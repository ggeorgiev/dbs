//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.hpp"
#include "doim/fs/fs_file.hpp"
#include <gtest/gtest.h>
#include <unordered_set>

TEST(CxxProgramTest, foo)
{
    dom::CxxProgram cxxProgram;

    std::unordered_set<doim::FsFileSPtr> files;
    cxxProgram.updateCxxFilesList(files);
}
