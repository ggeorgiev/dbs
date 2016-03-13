//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "im/initialization_manager.hpp"
#include <cppformat/format.h> // IWYU pragma: keep
#include <spdlog/spdlog.h>    // IWYU pragma: export
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

#define ILOG(...) \
    dbslog::gLogger->info(__VA_ARGS__) << " @ " << __FILE__ << ":" << __LINE__;

#define DLOG(...) \
    dbslog::gLogger->debug(__VA_ARGS__) << " @ " << __FILE__ << ":" << __LINE__;

#define TLOG(...) \
    dbslog::gLogger->trace(__VA_ARGS__) << " @ " << __FILE__ << ":" << __LINE__;
