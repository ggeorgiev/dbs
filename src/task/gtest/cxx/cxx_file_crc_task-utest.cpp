//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_file.h"
#include "task/cxx/cxx_source_crc_task.h"
#include "task/tpool.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
#include <fmt/format.h>
#include <memory>
#include <ostream>

class CxxFileCrcTaskTest : public ::testing::Test
{
public:
    CxxFileCrcTaskTest()
    {
    }

    void SetUp()
    {
        mCxxDirectory = doim::FsDirectory::obtain(testing::gTestResourceDirectory, "cxx");
        mFsSimpleCxx = doim::FsFile::obtain(mCxxDirectory, "simple.cxx");
        mFsIncludesCxx = doim::FsFile::obtain(mCxxDirectory, "includes.cxx");
        mFsUser1H = doim::FsFile::obtain(mCxxDirectory, "user1.h");
        mFsUser2H = doim::FsFile::obtain(mCxxDirectory, "user2.h");

        mEmptyCxxHeaderSet = doim::CxxHeaderSet::make();
        mEmptyCxxHeaderSet = doim::CxxHeaderSet::unique(mEmptyCxxHeaderSet);
        mEmptyCxxIncludeDirectorySet = doim::CxxIncludeDirectorySet::make();
        mEmptyCxxIncludeDirectorySet =
            doim::CxxIncludeDirectorySet::unique(mEmptyCxxIncludeDirectorySet);
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

TEST_F(CxxFileCrcTaskTest, SLOW_simple)
{
    auto cxxFile =
        doim::CxxFile::unique(mFsSimpleCxx, mEmptyCxxIncludeDirectorySet, nullptr);
    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxFile,
                                             nullptr);
    task::gTPool->ensureScheduled(task);
    ASSERT_OKAY(task->join());

    EXPECT_EQ(0xe2ff549c235dc8a0, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxFileCrcTaskTest, notFoundInclude)
{
    auto cxxIncludeDirectory =
        doim::CxxIncludeDirectory::unique(doim::CxxIncludeDirectory::EType::kUser,
                                          mCxxDirectory,
                                          mEmptyCxxHeaderSet);

    auto cxxIncludeDirectories =
        doim::CxxIncludeDirectorySet::unique({cxxIncludeDirectory});

    auto cxxFile = doim::CxxFile::unique(mFsIncludesCxx, cxxIncludeDirectories, nullptr);

    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxFile,
                                             nullptr);
    task::gTPool->ensureScheduled(task);
    ASSERT_BANNED(kNotFound, task->join());
}

TEST_F(CxxFileCrcTaskTest, VERYSLOW_includeFromOneDirectory)
{
    auto cxxHeader1 = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                              doim::CxxHeader::EVisibility::kPublic,
                                              mFsUser1H,
                                              mEmptyCxxIncludeDirectorySet,
                                              nullptr);
    auto cxxHeader2 = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                              doim::CxxHeader::EVisibility::kPublic,
                                              mFsUser2H,
                                              mEmptyCxxIncludeDirectorySet,
                                              nullptr);

    auto cxxHeaders = doim::CxxHeaderSet::unique({cxxHeader1, cxxHeader2});

    auto cxxIncludeDirectory =
        doim::CxxIncludeDirectory::unique(doim::CxxIncludeDirectory::EType::kUser,
                                          mCxxDirectory,
                                          cxxHeaders);

    auto cxxIncludeDirectories =
        doim::CxxIncludeDirectorySet::unique({cxxIncludeDirectory});

    auto cxxFile = doim::CxxFile::unique(mFsIncludesCxx, cxxIncludeDirectories, nullptr);

    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxFile,
                                             nullptr);
    task::gTPool->ensureScheduled(task);
    ASSERT_OKAY(task->join());

    EXPECT_EQ(0x82263f301a3afe30, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxFileCrcTaskTest, VERYSLOW_includeFromTwoDirectories)
{
    // This test makes sure that including from having more than one include directory
    // with the same fs directory is working.

    auto cxxHeader2 = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                              doim::CxxHeader::EVisibility::kPublic,
                                              mFsUser2H,
                                              mEmptyCxxIncludeDirectorySet,
                                              nullptr);

    auto cxxHeaders2 = doim::CxxHeaderSet::unique({cxxHeader2});

    auto cxxIncludeDirectory2 =
        doim::CxxIncludeDirectory::unique(doim::CxxIncludeDirectory::EType::kUser,
                                          mCxxDirectory,
                                          cxxHeaders2);

    auto cxxIncludeDirectories2 =
        doim::CxxIncludeDirectorySet::unique({cxxIncludeDirectory2});

    auto cxxHeader1 = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                              doim::CxxHeader::EVisibility::kPublic,
                                              mFsUser1H,
                                              cxxIncludeDirectories2,
                                              nullptr);

    auto cxxHeaders1 = doim::CxxHeaderSet::unique({cxxHeader1});

    auto cxxIncludeDirectory1 =
        doim::CxxIncludeDirectory::unique(doim::CxxIncludeDirectory::EType::kUser,
                                          mCxxDirectory,
                                          cxxHeaders1);

    auto cxxIncludeDirectories1 =
        doim::CxxIncludeDirectorySet::unique({cxxIncludeDirectory1});

    auto cxxFile = doim::CxxFile::unique(mFsIncludesCxx, cxxIncludeDirectories1, nullptr);

    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxFile,
                                             nullptr);
    task::gTPool->ensureScheduled(task);
    ASSERT_OKAY(task->join());

    EXPECT_EQ(0x82263f301a3afe30, task->crc()) << std::hex << task->crc();
}
