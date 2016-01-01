//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "performance_arbiter.h"
#include "boost/chrono/process_cpu_clocks.hpp"

namespace testing
{
class TestPartResult;

PerformanceArbiter::PerformanceArbiter() : mMode(kObserve)
{
}

void PerformanceArbiter::OnTestProgramStart(const UnitTest& /*unit_test*/)
{
}

void PerformanceArbiter::OnTestIterationStart(const UnitTest& /*unit_test*/, int /*iteration*/)
{
}

void PerformanceArbiter::OnEnvironmentsSetUpStart(const UnitTest& /*unit_test*/)
{
}

void PerformanceArbiter::OnEnvironmentsSetUpEnd(const UnitTest& /*unit_test*/)
{
}

void PerformanceArbiter::OnTestCaseStart(const TestCase& /*test_case*/)
{
}

void PerformanceArbiter::OnTestStart(const TestInfo& /*test_info*/)
{
    boost::chrono::process_cpu_clock::time_point now = boost::chrono::process_cpu_clock::now();
}

void PerformanceArbiter::OnTestPartResult(const TestPartResult& /*result*/)
{
}

void PerformanceArbiter::OnTestEnd(const TestInfo& /*test_info*/)
{
}

void PerformanceArbiter::OnTestCaseEnd(const TestCase& /*test_case*/)
{
}

void PerformanceArbiter::OnEnvironmentsTearDownStart(const UnitTest& /*unit_test*/)
{
}

void PerformanceArbiter::OnEnvironmentsTearDownEnd(const UnitTest& /*unit_test*/)
{
}

void PerformanceArbiter::OnTestIterationEnd(const UnitTest& /*unit_test*/, int /*iteration*/)
{
}

void PerformanceArbiter::OnTestProgramEnd(const UnitTest& /*unit_test*/)
{
}
} // namespace testing
