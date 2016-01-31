//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "log/log.h"
#include <boost/current_function.hpp>

namespace dbslog
{
class LogFunction
{
public:
    LogFunction(const char* file, int line, const char* functionName)
        : mFunctionName(functionName)
    {
        gLogger->trace("^ {} @ {}:{}", mFunctionName, file, line);
    }

    ~LogFunction()
    {
        gLogger->trace("$ {}", mFunctionName);
    }

private:
    const char* mFunctionName;
};
}

#define TLOG_FUNCTION \
    dbslog::LogFunction logFunction(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION);
