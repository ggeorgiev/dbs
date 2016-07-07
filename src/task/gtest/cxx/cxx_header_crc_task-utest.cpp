//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_header.h"
#include "task/cxx/cxx_source_crc_task.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest.h>
#include "fmt/format.h"
#include <memory>
#include <ostream>

class CxxHeaderCrcTaskTest : public ::testing::Test
{
public:
    CxxHeaderCrcTaskTest()
    {
    }

    void SetUp()
    {
        mCxxDirectory = doim::FsDirectory::obtain(testing::gResourceDirectory, "cxx");
        mFsSimpleCxx = doim::FsFile::obtain(mCxxDirectory, "simple.cxx");
        mFsIncludesCxx = doim::FsFile::obtain(mCxxDirectory, "includes.cxx");
        mFsUserH = doim::FsFile::obtain(mCxxDirectory, "user.h");

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
    doim::FsFileSPtr mFsUserH;

    doim::CxxHeaderSetSPtr mEmptyCxxHeaderSet;
    doim::CxxIncludeDirectorySetSPtr mEmptyCxxIncludeDirectorySet;
};

TEST_F(CxxHeaderCrcTaskTest, simple)
{
    auto cxxHeader = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                             doim::CxxHeader::EVisibility::kPublic,
                                             mFsSimpleCxx,
                                             mEmptyCxxIncludeDirectorySet,
                                             nullptr);

    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxHeader,
                                             nullptr);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0xe2ff549c235dc8a0, task->crc()) << std::hex << task->crc();
}

TEST_F(CxxHeaderCrcTaskTest, notFoundInclude)
{
    auto cxxIncludeDirectory =
        doim::CxxIncludeDirectory::unique(doim::CxxIncludeDirectory::EType::kUser,
                                          mCxxDirectory,
                                          mEmptyCxxHeaderSet);

    auto cxxIncludeDirectories =
        doim::CxxIncludeDirectorySet::unique({cxxIncludeDirectory});

    auto cxxHeader = doim::CxxHeader::unique(doim::CxxHeader::EType::kUser,
                                             doim::CxxHeader::EVisibility::kPublic,
                                             mFsIncludesCxx,
                                             cxxIncludeDirectories,
                                             nullptr);

    auto task = task::CxxSourceCrcTask::make(task::CxxSourceCrcTask::EDepth::kAll,
                                             cxxHeader,
                                             nullptr);
    ASSERT_BANNED(kNotFound, (*task)());
}
