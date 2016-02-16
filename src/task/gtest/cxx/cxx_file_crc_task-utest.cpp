//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h" // IWYU pragma: keep
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
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
        mFsUser1H = doim::gManager->obtainFile(mCxxDirectory, "user1.h");
        mFsUser2H = doim::gManager->obtainFile(mCxxDirectory, "user2.h");

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
    doim::FsFileSPtr mFsUser1H;
    doim::FsFileSPtr mFsUser2H;

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

TEST_F(CxxFileCrcTaskTest, SLOW_includeFromOneDirectory)
{
    auto cxxHeader1 = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                          mFsUser1H,
                                          mEmptyCxxIncludeDirectorySet));
    auto cxxHeader2 = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                          mFsUser2H,
                                          mEmptyCxxIncludeDirectorySet));

    auto cxxHeaders = std::make_shared<doim::CxxHeaderSet>();
    cxxHeaders->insert(cxxHeader1);
    cxxHeaders->insert(cxxHeader2);
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

    EXPECT_EQ(0xb7e440100a03d2c5, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxFileCrcTaskTest, includeFromTwoDirectories)
{
    // This test makes sure that including from having more than one include directory
    // with the same fs directory is working.

    auto cxxHeader2 = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                          mFsUser2H,
                                          mEmptyCxxIncludeDirectorySet));

    auto cxxHeaders2 = std::make_shared<doim::CxxHeaderSet>();
    cxxHeaders2->insert(cxxHeader2);
    cxxHeaders2 = doim::gManager->unique(cxxHeaders2);

    auto cxxIncludeDirectory2 = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   mCxxDirectory,
                                   cxxHeaders2);

    auto cxxIncludeDirectories2 = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories2->insert(doim::gManager->unique(cxxIncludeDirectory2));
    cxxIncludeDirectories2 = doim::gManager->unique(cxxIncludeDirectories2);

    auto cxxHeader1 = doim::gManager->unique(
        std::make_shared<doim::CxxHeader>(doim::CxxHeader::Type::kUser,
                                          mFsUser1H,
                                          cxxIncludeDirectories2));

    auto cxxHeaders1 = std::make_shared<doim::CxxHeaderSet>();
    cxxHeaders1->insert(cxxHeader1);
    cxxHeaders1 = doim::gManager->unique(cxxHeaders1);

    auto cxxIncludeDirectory1 = std::make_shared<
        doim::CxxIncludeDirectory>(doim::CxxIncludeDirectory::Type::kUser,
                                   mCxxDirectory,
                                   cxxHeaders1);

    auto cxxIncludeDirectories1 = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories1->insert(doim::gManager->unique(cxxIncludeDirectory1));
    cxxIncludeDirectories1 = doim::gManager->unique(cxxIncludeDirectories1);

    auto cxxFile =
        std::make_shared<doim::CxxFile>(mFsIncludesCxx, cxxIncludeDirectories1);
    auto task = std::make_shared<task::CxxFileCrcTask>(cxxFile);
    ASSERT_OKAY((*task)());

    EXPECT_EQ(0xb7e440100a03d2c5, task->crc()) << std::hex << task->crc();
}
