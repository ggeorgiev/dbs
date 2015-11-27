#include "im/initialization_manager.hpp"

#include "time_monitor.h"
#include "performance_arbiter.h"

#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
    im::InitializationManager im;

    testing::TimeMonitor* timeMonitor = new testing::TimeMonitor();
    testing::PerformanceArbiter* performanceArbiter = new testing::PerformanceArbiter();

    testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
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
