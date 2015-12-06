//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cpp/cpp_program.hpp"

#include "doim/fs/fs_file.hpp"

#include <gtest/gtest.h>

#include <unordered_set>

TEST(CppProgramTest, foo)
{
    dom::CppProgram cppProgram;

    std::unordered_set<doim::FsFileSPtr> files;
    cppProgram.updateCppFiles(files);
}
