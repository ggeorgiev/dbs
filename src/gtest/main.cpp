//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.hpp"
#include "doim/manager.h"
#include "im/initialization_manager.hpp"
#include "gtest/gtest.h"
#include "gtest/performance_arbiter.h"
#include "gtest/time_monitor.h"
#include <iosfwd>
#include <memory>
#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

namespace testing
{
std::string gIntermittentDirectory;
doim::FsDirectorySPtr gIntermittentFsDirectory;

doim::FsDirectorySPtr gTestResourceDirectory;
} // namespace testing

int main(int argc, char* argv[])
{
    im::InitializationManager im;

    char current[FILENAME_MAX];
    if (getcwd(current, sizeof(current)) == nullptr)
        return errno;

    auto cwd = doim::gManager->obtainDirectory(nullptr, current);
    testing::gIntermittentFsDirectory =
        doim::gManager->obtainDirectory(cwd, "build/test");
    testing::gIntermittentDirectory = testing::gIntermittentFsDirectory->path();

    testing::gTestResourceDirectory =
        doim::gManager->obtainDirectory(cwd, "test_resource");

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
