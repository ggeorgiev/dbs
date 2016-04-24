//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "im/initialization_manager.hpp"
#include <cppformat/format.h> // IWYU pragma: keep
#include <spdlog/logger.h>
#include <spdlog/spdlog.h> // IWYU pragma: export
#include <spdlog/details/logger_impl.h>
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

#define LOG(x) x // << " @ " << __FILE__ << ":" << __LINE__

#define ILOG(...) LOG(dbslog::gLogger->info(__VA_ARGS__))
#define DLOG(...) LOG(dbslog::gLogger->debug(__VA_ARGS__))
#define TLOG(...) LOG(dbslog::gLogger->trace(__VA_ARGS__))
