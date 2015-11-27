#include "time_monitor.h"

#include <limits>

namespace testing
{
TimeMonitor::TimeMonitor() : mMode(kObserve)
{
}

TimeMonitor::~TimeMonitor()
{
}

static const char kPerformance[] = "PERFORMANCE_";

size_t TimeMonitor::limit(const TestInfo& test_info)
{
    if (::strncmp(test_info.name(), kPerformance, sizeof(kPerformance) - 1) == 0)
        return std::numeric_limits<size_t>::max();
    return 15U;
}

bool TimeMonitor::reset()
{
    if (mFailedFilter.empty())
        return false;
    if (mMode == kReportError)
        return false;

    mMode = kReportError;
    GTEST_FLAG(filter) = mFailedFilter;
    return true;
}

void TimeMonitor::OnTestProgramStart(const UnitTest& unit_test)
{
    mFailedFilter.clear();
}

void TimeMonitor::OnTestIterationStart(const UnitTest& unit_test, int iteration)
{
}

void TimeMonitor::OnEnvironmentsSetUpStart(const UnitTest& unit_test)
{
}

void TimeMonitor::OnEnvironmentsSetUpEnd(const UnitTest& unit_test)
{
}

void TimeMonitor::OnTestCaseStart(const TestCase& test_case)
{
}

void TimeMonitor::OnTestStart(const TestInfo& test_info)
{
}

void TimeMonitor::OnTestPartResult(const TestPartResult& result)
{
}

void TimeMonitor::OnTestEnd(const TestInfo& test_info)
{
    size_t expected = limit(test_info);

    if ((size_t)test_info.result()->elapsed_time() <= expected)
        return;

    if (mMode == kObserve)
    {
        if (!mFailedFilter.empty())
            mFailedFilter += ":";

        mFailedFilter += test_info.test_case_name();
        mFailedFilter += ".";
        mFailedFilter += test_info.name();
        return;
    }

    GTEST_FAIL() << "This test took " << test_info.result()->elapsed_time() << " ms "
                 << "instead the expected " << expected << " ms.";
}

void TimeMonitor::OnTestCaseEnd(const TestCase& test_case)
{
}

void TimeMonitor::OnEnvironmentsTearDownStart(const UnitTest& unit_test)
{
}

void TimeMonitor::OnEnvironmentsTearDownEnd(const UnitTest& unit_test)
{
}

void TimeMonitor::OnTestIterationEnd(const UnitTest& unit_test, int iteration)
{
}

void TimeMonitor::OnTestProgramEnd(const UnitTest& unit_test)
{
}
}