//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.hpp"
#include "doim/cxx/cxx_file.hpp"
#include "doim/cxx/cxx_header.hpp"
#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include "gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <ios>
#include <memory>
#include <unordered_set>

TEST(SearchTaskTest, simple)
{
    auto file =
        doim::gManager->obtainFile(testing::gTestResourceDirectory, "cxx/simple.cxx");
    auto cxxIncludeDirectories =
        doim::gManager->unique(std::make_shared<doim::CxxIncludeDirectorySet>());
    auto cxxHeaders = doim::gManager->unique(std::make_shared<doim::CxxHeaderSet>());

    auto cxxFile =
        std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories, cxxHeaders);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x9f9163a56c247be9, task->crc()) << std::hex << task->crc();
}

TEST(SearchTaskTest, includes)
{
    auto directory =
        doim::gManager->obtainDirectory(testing::gTestResourceDirectory, "cxx");

    auto file = doim::gManager->obtainFile(directory, "includes.cxx");
    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();

    auto cxxIncludeDirectory = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser, directory);

    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));

    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);
    auto cxxHeaders = doim::gManager->unique(std::make_shared<doim::CxxHeaderSet>());

    auto cxxFile =
        std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories, cxxHeaders);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);
    ASSERT_BANNED(kNotFound, (*task)());

    auto user = doim::gManager->obtainFile(directory, "user.h");

    ASSERT_BANNED(kNotFound, (*task)());

    auto cxxHeader = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(user, cxxIncludeDirectories));

    auto cxxUserHeaders = std::make_shared<doim::CxxHeaderSet>();
    cxxUserHeaders->insert(cxxHeader);
    cxxUserHeaders = doim::gManager->unique(cxxUserHeaders);

    cxxFile =
        std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories, cxxUserHeaders);

    task = std::make_shared<task::CxxFileCrcTask>(cxxFile);

    ASSERT_OKAY((*task)());

    // EXPECT_EQ(0x9dd10acde5de7bda, task->crc()) << std::hex << task->crc();
}
