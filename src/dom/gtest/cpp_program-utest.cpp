//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cpp/cpp_program.hpp"

#include <gtest/gtest.h>

TEST(CppProgramTest, foo)
{
    dom::CppProgram cppProgram;

    std::unordered_set<dom::FsFileSPtr> files;
    cppProgram.updateCppFiles(files);
}
