//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <str>
#include <stddef.h>

TEST(DbsParserTest, SLOW_CxxLibrary)
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

TEST(DbsParserTest, SLOW_CxxLibraryCxxHeader)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    auto cxxLibraryCxxHeaderPublicObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                             mDbsDirectory,
                             "cxx_header_public");

    auto cxxLibraryCxxHeaderPublic =
        dom::CxxLibrary::find(cxxLibraryCxxHeaderPublicObject);
    ASSERT_NE(nullptr, cxxLibraryCxxHeaderPublic);
}

TEST(DbsParserTest, SLOW_CxxLibraryCxxLibrary)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    auto cxxLibraryCxxLibrariesObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                             mDbsDirectory,
                             "cxx_libraries");

    auto cxxLibraryCxxLibraries = dom::CxxLibrary::find(cxxLibraryCxxLibrariesObject);
    ASSERT_NE(nullptr, cxxLibraryCxxLibraries);

    ASSERT_EQ(1, cxxLibraryCxxLibraries->cxxLibraries().size());
}

TEST(DbsParserTest, SLOW_CxxLibraryCxxFile)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    auto cxxLibraryCxxFilesObject = doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                                                         mDbsDirectory,
                                                         "cxx_files");

    auto cxxLibraryCxxFiles = dom::CxxLibrary::find(cxxLibraryCxxFilesObject);
    ASSERT_NE(nullptr, cxxLibraryCxxFiles);

    ASSERT_EQ(1, cxxLibraryCxxFiles->cxxFilesList().size());
}

TEST(DbsParserTest, SLOW_CxxLibraryBinary)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    auto cxxLibraryBinaryObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "binary");

    auto cxxLibraryBinary = dom::CxxLibrary::find(cxxLibraryBinaryObject);
    ASSERT_NE(nullptr, cxxLibraryBinary);
    ASSERT_EQ("dbs/libfoo.a",
              cxxLibraryBinary->binary()->path(testing::gTestResourceDirectory));
}