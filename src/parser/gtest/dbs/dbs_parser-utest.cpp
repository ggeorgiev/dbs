//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <str>
#include <stddef.h>

TEST(DbsParserTest, _parse)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    auto cxxLibraryEmptyObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "empty");

    auto cxxLibraryEmpty = dom::CxxLibrary::find(cxxLibraryEmptyObject);
    ASSERT_NE(nullptr, cxxLibraryEmpty);

    // System
    auto cxxLibrarySystemObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "system");

    auto cxxLibrarySystem = dom::CxxLibrary::find(cxxLibrarySystemObject);
    ASSERT_NE(nullptr, cxxLibrarySystem);
    EXPECT_EQ(dom::CxxLibrary::EType::kSystem, cxxLibrarySystem->type());

    // User
    auto cxxLibraryUserObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "user");

    auto cxxLibraryUser = dom::CxxLibrary::find(cxxLibraryUserObject);
    ASSERT_NE(nullptr, cxxLibraryUser);
    EXPECT_EQ(dom::CxxLibrary::EType::kUser, cxxLibraryUser->type());
}
