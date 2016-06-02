//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/tag/tag.h"
#include "doim/tag/tag_expression.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <memory>

TEST(DbsConfigParserTest, SLOW_verbose)
{
    auto mDbsConfigDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs/config");
    auto config = doim::FsFile::obtain(mDbsConfigDirectory, "verbose.dbs");

    parser::DbsConfigParser parser;
    ASSERT_OKAY(parser.parse(config));

    auto task = doim::TagExpression::unique(doim::TagExpression::ETurn::kOn,
                                            doim::TagSet::unique(doim::TagSet::make(
                                                doim::TagSet{doim::gTaskTag})),
                                            nullptr);

    auto parse = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                             doim::TagSet::unique(doim::TagSet::make(
                                                 doim::TagSet{doim::gParseTag})),
                                             task);

    auto crc = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                           doim::TagSet::unique(doim::TagSet::make(
                                               doim::TagSet{doim::gCrcTag})),
                                           parse);

    auto db = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                          doim::TagSet::unique(doim::TagSet::make(
                                              doim::TagSet{doim::gDbTag})),
                                          crc);

    auto sys = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                           doim::TagSet::unique(doim::TagSet::make(
                                               doim::TagSet{doim::gSysTag})),
                                           db);

    ASSERT_EQ(sys, parser.mTagExpression);
}
