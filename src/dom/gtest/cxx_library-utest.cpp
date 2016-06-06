//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.h"
#include "parser/dbs/dbs_parser.h"
#include "doim/cxx/cxx_header.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "doim/set.hpp"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <memory>
#include <str>
#include <unordered>

static void parse(string name)
{
    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");
    auto file = doim::FsFile::obtain(mDbsDirectory, name);

    parser::DbsParser parser;
    parser.parse(file);
}

TEST(CxxLibraryTest, SLOW_CxxLibraryHeaders)
{
    parse("cxx_library_headers.dbs");

    auto mDbsDirectory =
        doim::FsDirectory::obtain(testing::gTestResourceDirectory, "dbs");

    auto cxxLibraryLib2Object = doim::Object::obtain(doim::Object::EType::kCxxLibrary,
                                                     mDbsDirectory,
                                                     "headers_lib2");

    auto cxxLibraryLib2 = dom::CxxLibrary::find(cxxLibraryLib2Object);
    ASSERT_NE(nullptr, cxxLibraryLib2);

    const auto& publicHeaders = cxxLibraryLib2->publicCxxHeaders(mDbsDirectory);
    ASSERT_EQ(1, publicHeaders->size());

    const auto& recursivePublicHeaders =
        cxxLibraryLib2->recursivePublicCxxHeaders(mDbsDirectory);
    ASSERT_EQ(2, recursivePublicHeaders->size());

    const auto& protectedHeaders = cxxLibraryLib2->protectedCxxHeaders(mDbsDirectory);
    ASSERT_EQ(1, protectedHeaders->size());

    const auto& privateHeaders = cxxLibraryLib2->privateCxxHeaders(mDbsDirectory);
    ASSERT_EQ(1, privateHeaders->size());

    const auto& visibleDirectories =
        cxxLibraryLib2->visibleCxxIncludeDirectories(mDbsDirectory);
    ASSERT_EQ(4, visibleDirectories->size());
}

TEST(CxxLibraryTest, cxxLibraries)
{
    dom::CxxLibrary library;

    const auto& libraries = library.cxxLibraries();
    ASSERT_EQ(0U, libraries.size());
}

TEST(CxxLibraryTest, recursiveCxxLibraries)
{
    dom::CxxLibrary library;

    const auto& libraries = library.recursiveCxxLibraries();
    ASSERT_EQ(0U, libraries.size());
}

TEST(CxxLibraryTest, publicCxxHeaders)
{
    dom::CxxLibrary library;

    const auto& directory = doim::FsDirectory::obtain(nullptr, "/foo/include");

    const auto& cxxHeaders1 = library.publicCxxHeaders(directory);
    ASSERT_EQ(0U, cxxHeaders1->size());

    doim::FsFileSet files;

    const auto& file = doim::FsFile::obtain(directory, "header.h");
    files.insert(file);

    ASSERT_OKAY(library.updateCxxHeaders(doim::CxxHeader::EVisibility::kPublic,
                                         directory,
                                         files));

    const auto& cxxHeaders2 = library.publicCxxHeaders(directory);
    ASSERT_EQ(1U, cxxHeaders2->size());

    const auto& cxxIncludeDirectories = library.cxxPublicIncludeDirectories(directory);
    ASSERT_EQ(1U, cxxIncludeDirectories.size());
}
