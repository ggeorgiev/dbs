//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <memory>
#include <ostream>
#include <unordered_set>

class CxxHeaderCrcTaskTest : public ::testing::Test
{
public:
    CxxHeaderCrcTaskTest()
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

TEST_F(CxxHeaderCrcTaskTest, simple)
{
    auto cxxHeader = doim::CxxHeader::make(doim::CxxHeader::Type::kUser,
                                           mFsSimpleCxx,
                                           mEmptyCxxIncludeDirectorySet);
    cxxHeader = doim::gManager->unique(cxxHeader);

    auto task = task::CxxHeaderCrcTask::make(cxxHeader, nullptr);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x9f9163a56c247be9, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxHeaderCrcTaskTest, notFoundInclude)
{
    auto cxxIncludeDirectory =
        doim::CxxIncludeDirectory::make(doim::CxxIncludeDirectory::EType::kUser,
                                        mCxxDirectory,
                                        mEmptyCxxHeaderSet);
    cxxIncludeDirectory = doim::gManager->unique(cxxIncludeDirectory);

    auto cxxIncludeDirectories = std::make_shared<doim::CxxIncludeDirectorySet>();
    cxxIncludeDirectories->insert(doim::gManager->unique(cxxIncludeDirectory));
    cxxIncludeDirectories = doim::gManager->unique(cxxIncludeDirectories);

    auto cxxHeader = doim::CxxHeader::make(doim::CxxHeader::Type::kUser,
                                           mFsIncludesCxx,
                                           cxxIncludeDirectories);
    cxxHeader = doim::gManager->unique(cxxHeader);
    auto task = task::CxxHeaderCrcTask::make(cxxHeader, nullptr);
    ASSERT_BANNED(kNotFound, (*task)());
}
