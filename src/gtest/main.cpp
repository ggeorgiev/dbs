#include "im/initialization_manager.hpp"

#include "gtest/test_resource.h"
#include "time_monitor.h"
#include "performance_arbiter.h"

#include "doim/manager.h"

#include "gtest/gtest.h"

namespace testing
{
doim::FsDirectorySPtr gTestResourceDirectory;
}

int main(int argc, char* argv[])
{
    im::InitializationManager im;

    char current[FILENAME_MAX];
    if (getcwd(current, sizeof(current)) == nullptr)
        return errno;

    auto cwd = doim::gManager->obtainDirectory(nullptr, current);
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
