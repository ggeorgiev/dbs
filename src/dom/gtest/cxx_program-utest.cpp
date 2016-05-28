//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"
#include "parser/dbs/dbs_parser.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <memory>
#include <str>

static void parse(string name)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto file = doim::FsFile::obtain(mDbsDirectory, name);

    parser::DbsParser parser;
    parser.parse(file);
}

TEST(CxxProgramTest, SLOW_CxxProgramHeaders)
{
    parse("cxx_program_headers.dbs");

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxProgramCxxLibrariesObject =
        doim::Object::obtain(doim::Object::EType::kCxxProgram,
                             mDbsDirectory,
                             "headers_prog1");

    auto cxxProgramCxxLibraries = dom::CxxProgram::find(cxxProgramCxxLibrariesObject);
    ASSERT_NE(nullptr, cxxProgramCxxLibraries);
}