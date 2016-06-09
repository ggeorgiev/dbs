//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "option/verbose.h"
#include "doim/tag/tag.h"
#include "log/log.h"

namespace logex
{
class DbsLogger;
typedef shared_ptr<DbsLogger> DbsLoggerSPtr;

class DbsLogger
{
public:
    static constexpr int rank()
    {
        return dbslog::rank() + im::InitializationManager::step();
    }

    template <typename... Args>
    void log(const doim::TagSetSPtr& tags, const Args&... args)
    {
        if (opt::gVerbose->isVisible(tags))
            ILOG(args...);
    }
};

extern DbsLoggerSPtr gLogger;

#define LOGEX(...) LOG(logex::gLogger->log(__VA_ARGS__))
}