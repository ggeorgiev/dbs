#pragma once

#include <gtest/gtest.h>

#include <boost/chrono/process_cpu_clocks.hpp>

#include <unordered_map>

namespace testing
{

class PerformanceArbiter : public TestEventListener
{
public:
    PerformanceArbiter();
    ~PerformanceArbiter() override;

    void OnTestProgramStart(const UnitTest& unit_test) override;
    void OnTestIterationStart(const UnitTest& unit_test, int iteration) override;
    void OnEnvironmentsSetUpStart(const UnitTest& unit_test) override;
    void OnEnvironmentsSetUpEnd(const UnitTest& unit_test) override;
    void OnTestCaseStart(const TestCase& test_case) override;
    void OnTestStart(const TestInfo& test_info) override;
    void OnTestPartResult(const TestPartResult& result) override;
    void OnTestEnd(const TestInfo& test_info) override;
    void OnTestCaseEnd(const TestCase& test_case) override;
    void OnEnvironmentsTearDownStart(const UnitTest& unit_test) override;
    void OnEnvironmentsTearDownEnd(const UnitTest& unit_test) override;
    void OnTestIterationEnd(const UnitTest& unit_test, int iteration) override;
    void OnTestProgramEnd(const UnitTest& unit_test) override;

    enum Mode
    {
        kObserve,
        kReportError
    };

    typedef std::unordered_map<std::string, boost::chrono::process_cpu_clock::time_point> TrackMap;

private:
    Mode mMode;
    TrackMap mStartMap;
    TrackMap mEndMap;
};
}