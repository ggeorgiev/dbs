//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_header.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "doim/set.hpp"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <memory>
#include <str>

static void parse()
{
    static bool parsed = false;
    if (parsed)
        return;

    auto mDbsConfigDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs/config");
    auto config = doim::FsFile::obtain(mDbsConfigDirectory, "config.dbs");

    parser::DbsConfigParser parser;
    parser.parse(config);

    parsed = true;
}

TEST(DbsConfigParserTest, SLOW_verbose)
{
    parse();
}
