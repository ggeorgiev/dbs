//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "log/log.h"
#include "log/log_function.hpp"
#include <gtest/gtest.h>
#include <spdlog/logger.h>
#include <spdlog/details/line_logger_fwd.h>
#include <spdlog/details/line_logger_impl.h>
#include <spdlog/details/logger_impl.h>
#include <memory>

TEST(Log, basic)
{
    dbslog::gLogger->info() << "test";
    dbslog::gLogger->info("test");
}

void foo()
{
    TLOG_FUNCTION;
    DLOG("test");
}
void bar()
{
    TLOG_FUNCTION;
    foo();
}

void baz()
{
    TLOG_FUNCTION;
    bar();
}

TEST(Log, SLOW_LOG_FUNCTION)
{
    TLOG_FUNCTION;
    baz();
}
