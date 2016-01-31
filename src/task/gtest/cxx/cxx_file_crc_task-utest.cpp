//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "doim/cxx/cxx_file.hpp"
#include "doim/cxx/cxx_header.hpp"
#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include "gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

class CxxFileCrcTaskTest : public ::testing::Test
{
public:
    CxxFileCrcTaskTest()
    {
    }

    void SetUp()
    {
        mCxxDirectory =
            doim::gManager->obtainDirectory(testing::gTestResourceDirectory, "cxx");
        mFsSimpleCxx = doim::gManager->obtainFile(mCxxDirectory, "simple.cxx");
        mFsIncludesCxx = doim::gManager->obtainFile(mCxxDirectory, "includes.cxx");
        mFsUserH = doim::gManager->obtainFile(mCxxDirectory, "user.h");

        mEmptyCxxHeaderSet =
            doim::gManager->unique(std::make_shared<doim::CxxHeaderSet>());
        mEmptyCxxIncludeDirectorySet =
            doim::gManager->unique(std::make_shared<doim::CxxIncludeDirectorySet>());
    }

    void TearDown()
    {
    }

    doim::FsDirectorySPtr mCxxDirectory;
    doim::FsFileSPtr mFsSimpleCxx;
    doim::FsFileSPtr mFsIncludesCxx;
    doim::FsFileSPtr mFsUserH;

    doim::CxxHeaderSetSPtr mEmptyCxxHeaderSet;
    doim::CxxIncludeDirectorySetSPtr mEmptyCxxIncludeDirectorySet;
};

TEST_F(CxxFileCrcTaskTest, simple)
{
    auto cxxFile =
        std::make_shared<doim::CxxFile>(mFsSimpleCxx, mEmptyCxxIncludeDirectorySet);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x9f9163a56c247be9, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxFileCrcTaskTest, notFoundInclude)
{
    auto cxxIncludeDirectory = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   mCxxDirectory,
                                   mEmptyCxxHeaderSet);

    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));
    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);

    auto cxxFile = std::make_shared<doim::CxxFile>(mFsIncludesCxx, cxxIncludeDirectories);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);
    ASSERT_BANNED(kNotFound, (*task)());
}

TEST_F(CxxFileCrcTaskTest, include)
{
    auto cxxHeader = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                          mFsUserH,
                                          mEmptyCxxIncludeDirectorySet));

    auto cxxHeaders = std::make_shared<doim::CxxHeaderSet>();
    cxxHeaders->insert(cxxHeader);
    cxxHeaders = doim::gManager->unique(cxxHeaders);

    auto cxxIncludeDirectory = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   mCxxDirectory,
                                   cxxHeaders);

    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));
    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);

    auto cxxFile = std::make_shared<doim::CxxFile>(mFsIncludesCxx, cxxIncludeDirectories);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);
    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x5aad46785dc264df, task->crc()) << std::hex << task->crc();
}
