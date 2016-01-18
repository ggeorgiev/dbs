//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_header.hpp"
#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include "gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>

TEST(CxxHeaderCrcTaskTest, simple)
{
    auto file =
        doim::gManager->obtainFile(testing::gTestResourceDirectory, "cxx/simple.cxx");
    auto cxxIncludeDirectories =
        doim::gManager->unique(std::make_shared<doim::CxxIncludeDirectorySet>());
    auto cxxHeaders = doim::gManager->unique(std::make_shared<doim::CxxHeaderSet>());

    auto cxxHeader = std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                                       file,
                                                       cxxIncludeDirectories);
    auto task = std::make_shared<task::CxxHeaderCrcTask>(cxxHeader, cxxHeaders);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x9f9163a56c247be9, task->crc()) << std::hex << task->crc();
}
/*
TEST(CxxHeaderCrcTaskTest, notFoundInclude)
{
    auto directory =
        doim::gManager->obtainDirectory(testing::gTestResourceDirectory, "cxx");

    auto file = doim::gManager->obtainFile(directory, "includes.cxx");

    auto headers = doim::gManager->unique(std::make_shared<doim::FsFileSet>());

    auto cxxIncludeDirectory = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   directory,
                                   headers);

    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));

    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);
    auto cxxHeaders = doim::gManager->unique(std::make_shared<doim::CxxHeaderSet>());

    auto cxxFile =
        std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories, cxxHeaders);
    auto task = std::make_shared<task::CxxHeaderCrcTask>(cxxFile);
    ASSERT_BANNED(kNotFound, (*task)());

    auto user = doim::gManager->obtainFile(directory, "user.h");

    ASSERT_BANNED(kNotFound, (*task)());
}

TEST(CxxHeaderCrcTaskTest, include)
{
    auto directory =
        doim::gManager->obtainDirectory(testing::gTestResourceDirectory, "cxx");
    auto file = doim::gManager->obtainFile(directory, "includes.cxx");
    auto user = doim::gManager->obtainFile(directory, "user.h");
    auto headers = std::make_shared<doim::FsFileSet>();
    headers->insert(user);
    headers = doim::gManager->unique(headers);

    auto cxxIncludeDirectory = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   directory,
                                   headers);

    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));

    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);
    auto cxxHeader = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(user, cxxIncludeDirectories));

    auto cxxHeaders = std::make_shared<doim::CxxHeaderSet>();
    cxxHeaders->insert(cxxHeader);
    cxxHeaders = doim::gManager->unique(cxxHeaders);

    auto cxxFile =
        std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories, cxxHeaders);
    auto task = std::make_shared<task::CxxHeaderCrcTask>(cxxFile);
    ASSERT_OKAY((*task)());

    // EXPECT_EQ(0x9dd10acde5de7bda, task->crc()) << std::hex << task->crc();
}
*/
