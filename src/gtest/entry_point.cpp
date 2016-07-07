//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "db/database.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include "gtest/gtest.h"
#include "gtest/performance_arbiter.h"
#include "gtest/time_monitor.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/hana/for_each.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <str>
#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

namespace testing
{
string gIntermittentDirectory;
doim::FsDirectorySPtr gIntermittentFsDirectory;

doim::FsDirectorySPtr gResourceDirectory;
doim::FsDirectorySPtr gTempDirectory =
    doim::FsDirectory::global(nullptr,
                              boost::filesystem::temp_directory_path().string() + "test",
                              gTempDirectory);

int run(int argc, char* argv[])
{
    im::InitializationManager im;

    char current[FILENAME_MAX];
    if (getcwd(current, sizeof(current)) == nullptr)
        return errno;

    auto cwd = doim::FsDirectory::obtain(nullptr, current);
    testing::gIntermittentFsDirectory = doim::FsDirectory::obtain(cwd, "build/test");
    testing::gIntermittentDirectory = testing::gIntermittentFsDirectory->path();

    auto db = doim::FsDirectory::obtain(testing::gIntermittentFsDirectory, "build/db");
    ECode code = db::gDatabase->open(db->path());
    if (code != err::kSuccess)
    {
        EHReset;
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    testing::gResourceDirectory = doim::FsDirectory::obtain(cwd, "test_resource");

    auto timeMonitor = new testing::TimeMonitor();
    auto performanceArbiter = new testing::PerformanceArbiter();

    testing::TestEventListeners& listeners =
        testing::UnitTest::GetInstance()->listeners();
    listeners.Append(timeMonitor);
    listeners.Append(performanceArbiter);

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    while (result == 0)
    {
        if (!timeMonitor->reset())
            break;
        result = RUN_ALL_TESTS();
    }

    return result;
}
}