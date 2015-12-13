#include "task/cxx/cxx_file_crc_task.hpp"

#include "doim/manager.h"

#include "gtest/test_resource.h"

#include "gtest/err.h"

#include <gtest/gtest.h>

#include <bitset>

TEST(SearchTaskTest, simple)
{
    doim::FsFileSPtr file =
        doim::gManager->obtainFile(testing::gTestResourceDirectory, "cxx/simple.cxx");
    doim::CxxIncludeDirectorySetSPtr cxxIncludeDirectories =
        doim::gManager->unique(std::make_shared<doim::CxxIncludeDirectorySet>());

    auto cxxFile = std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories);
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

    cxxIncludeDirectories->insert(
        doim::gManager->obtainCxxIncludeDirectory(doim::CxxIncludeDirectory::Type::kUser,
                                                  directory));

    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);

    auto cxxFile = std::make_shared<doim::CxxFile>(file, cxxIncludeDirectories);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);

    // ASSERT_OKAY((*task)());

    // EXPECT_EQ(0x9dd10acde5de7bda, task->crc()) << std::hex << task->crc();
}
