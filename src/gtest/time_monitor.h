//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "gtest/framework.h"
#include <str>
#include <stddef.h>

namespace testing
{
class TimeMonitor : public TestEventListener
{
public:
    TimeMonitor();
    ~TimeMonitor() = default;

    static size_t limit(const TestInfo& test_info);

    bool reset();

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

private:
    Mode mMode;
    string mFailedFilter;
};
}
