//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
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

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto main = doim::FsFile::obtain(mDbsDirectory, "main.dbs");

    parser::DbsParser parser;
    parser.parse(main);

    parsed = true;
}

TEST(DbsParserTest, SLOW_CxxLibrary)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

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
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryCxxHeaderPublicObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                             mDbsDirectory,
                             "cxx_header_public");

    auto cxxLibraryCxxHeaderPublic =
        dom::CxxLibrary::find(cxxLibraryCxxHeaderPublicObject);
    ASSERT_NE(nullptr, cxxLibraryCxxHeaderPublic);

    auto headerFiles =
        cxxLibraryCxxHeaderPublic->headerFiles(doim::CxxHeader::EVisibility::kPublic);
    ASSERT_NE(nullptr, headerFiles);
    ASSERT_EQ(1, headerFiles->size());
}

TEST(DbsParserTest, SLOW_CxxLibraryCxxLibrary)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

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
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryCxxFilesObject = doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                                                         mDbsDirectory,
                                                         "cxx_files");

    auto cxxLibraryCxxFiles = dom::CxxLibrary::find(cxxLibraryCxxFilesObject);
    ASSERT_NE(nullptr, cxxLibraryCxxFiles);

    ASSERT_EQ(1, cxxLibraryCxxFiles->cxxFilesList().size());
}

TEST(DbsParserTest, SLOW_CxxLibraryProtobufFile)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryProtobufFilesObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                             mDbsDirectory,
                             "cxx_protobufs");

    auto cxxLibraryProtobufFiles = dom::CxxLibrary::find(cxxLibraryProtobufFilesObject);
    ASSERT_NE(nullptr, cxxLibraryProtobufFiles);

    ASSERT_EQ(1, cxxLibraryProtobufFiles->protobufsList().size());
}

TEST(DbsParserTest, SLOW_CxxLibraryBinary)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryBinaryObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "binary");

    auto cxxLibraryBinary = dom::CxxLibrary::find(cxxLibraryBinaryObject);
    ASSERT_NE(nullptr, cxxLibraryBinary);
    ASSERT_NE(nullptr, cxxLibraryBinary->binary());
    ASSERT_EQ("dbs/libfoo.a",
              cxxLibraryBinary->binary()->path(testing::gTestResourceDirectory));
}

TEST(DbsParserTest, SLOW_CxxLibraryAll)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryAllObject =
        doim::Object::obtain(doim::Object::EType::kCxxLibrary, mDbsDirectory, "cxx_all");

    auto cxxLibraryAll = dom::CxxLibrary::find(cxxLibraryAllObject);
    ASSERT_EQ(1, cxxLibraryAll->cxxFilesList().size());
    ASSERT_EQ(1, cxxLibraryAll->cxxLibraries().size());
    ASSERT_NE(nullptr, cxxLibraryAll);
    ASSERT_NE(nullptr, cxxLibraryAll->binary());
    ASSERT_EQ("dbs/libfoo.a",
              cxxLibraryAll->binary()->path(testing::gTestResourceDirectory));
}

TEST(DbsParserTest, SLOW_CxxProgramEmpty)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxProgramEmptyObject =
        doim::Object::obtain(doim::Object::EType::kCxxProgram, mDbsDirectory, "empty");

    auto cxxProgramEmpty = dom::CxxProgram::find(cxxProgramEmptyObject);
    ASSERT_NE(nullptr, cxxProgramEmpty);
}

TEST(DbsParserTest, SLOW_CxxProgramCxxLibrary)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxProgramCxxLibrariesObject =
        doim::Object::obtain(doim::Object::EType::kCxxProgram,
                             mDbsDirectory,
                             "cxx_libraries");

    auto cxxProgramCxxLibraries = dom::CxxProgram::find(cxxProgramCxxLibrariesObject);
    ASSERT_NE(nullptr, cxxProgramCxxLibraries);

    ASSERT_EQ(1, cxxProgramCxxLibraries->cxxLibraries().size());
}

TEST(DbsParserTest, SLOW_CxxProgramCxxFile)
{
    parse();

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxProgramCxxFilesObject = doim::Object::obtain(doim::Object::EType::kCxxProgram,
                                                         mDbsDirectory,
                                                         "cxx_files");

    auto cxxProgramCxxFiles = dom::CxxProgram::find(cxxProgramCxxFilesObject);
    ASSERT_NE(nullptr, cxxProgramCxxFiles);

    ASSERT_EQ(1, cxxProgramCxxFiles->cxxFilesList().size());
}
