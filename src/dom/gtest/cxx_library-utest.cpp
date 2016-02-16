//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.hpp"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

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

    const auto& directory = doim::gManager->obtainDirectory(nullptr, "/foo/include");

    const auto& cxxHeaders1 = library.publicCxxHeaders(directory);
    ASSERT_EQ(0U, cxxHeaders1->size());

    std::unordered_set<doim::FsFileSPtr> files;

    const auto& file = doim::gManager->obtainFile(directory, "header.h");
    files.insert(file);

    ASSERT_OKAY(library.updateCxxPublicHeaders(directory, files));

    const auto& cxxHeaders2 = library.publicCxxHeaders(directory);
    ASSERT_EQ(1U, cxxHeaders2->size());

    const auto& cxxIncludeDirectory = library.cxxPublicIncludeDirectory(directory);
    ASSERT_NE(nullptr, cxxIncludeDirectory);
}

TEST(CxxLibraryTest, recursiveCxxHeaders)
{
    dom::CxxLibrary library;

    const auto& directory = doim::gManager->obtainDirectory(nullptr, "/foo/include");

    const auto& cxxHeaders1 = library.recursiveCxxHeaders(directory);
    ASSERT_EQ(0U, cxxHeaders1->size());

    std::unordered_set<doim::FsFileSPtr> files;

    const auto& file = doim::gManager->obtainFile(directory, "header.h");
    files.insert(file);

    ASSERT_OKAY(library.updateCxxPublicHeaders(directory, files));

    const auto& cxxHeaders2 = library.recursiveCxxHeaders(directory);
    ASSERT_EQ(1U, cxxHeaders2->size());
}
