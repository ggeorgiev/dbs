//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "im/initialization_manager.hpp"
#include <cppformat/format.h>
#include <spdlog/spdlog.h>
#include <memory>

namespace dbslog
{
inline int initialization_rank()
{
    return im::InitializationManager::rank_base() +
           im::InitializationManager::rank_step();
}

typedef spdlog::logger DbsLogger;
typedef std::shared_ptr<DbsLogger> DbsLoggerSPtr;

extern DbsLoggerSPtr gLogger;
}

#define DLOG(...) \
    dbslog::gLogger->debug(__VA_ARGS__) << " @ " << __FILE__ << ":" << __LINE__;

#define TLOG(...) \
    dbslog::gLogger->trace(__VA_ARGS__) << " @ " << __FILE__ << ":" << __LINE__;
