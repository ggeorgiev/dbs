#include "task/cxx/cxx_file_crc_task.hpp"

#include "doim/manager.h"

#include "gtest/test_resource.h"

#include "gtest/err.h"

#include <gtest/gtest.h>

#include <bitset>

TEST(SearchTaskTest, types)
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
