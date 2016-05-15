//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_header_crc_task.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/test_resource.h"
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
        mCxxDirectory = doim::FsDirectory::obtain(testing::gTestResourceDirectory, "cxx");
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
                                             mFsSimpleCxx,
                                             mEmptyCxxIncludeDirectorySet);

    auto task = task::CxxHeaderCrcTask::make(cxxHeader, nullptr);

    ASSERT_OKAY((*task)());

    EXPECT_EQ(0x9946caabb97e05b3, task->crc()) << std::hex << task->crc();
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
                                             mFsIncludesCxx,
                                             cxxIncludeDirectories);
    auto task = task::CxxHeaderCrcTask::make(cxxHeader, nullptr);
    ASSERT_BANNED(kNotFound, (*task)());
}
