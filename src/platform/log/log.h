//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "im/initialization_manager.hpp"
#include <fmt/format.h>                      // IWYU pragma: export
#include <spdlog/common.h>                   // IWYU pragma: export
#include <spdlog/logger.h>                   // IWYU pragma: export
#include <spdlog/spdlog.h>                   // IWYU pragma: export
#include <spdlog/details/line_logger_fwd.h>  // IWYU pragma: export
#include <spdlog/details/line_logger_impl.h> // IWYU pragma: export
#include <spdlog/details/logger_impl.h>      // IWYU pragma: export
#include <spdlog/details/spdlog_impl.h>      // IWYU pragma: export
#include <memory>

namespace dbslog
{
constexpr int rank()
{
    return im::InitializationManager::rank() + im::InitializationManager::step();
}

typedef spdlog::logger DbsLogger;
typedef shared_ptr<DbsLogger> DbsLoggerSPtr;

extern DbsLoggerSPtr gLogger;
}

#define LOG(x) x // << " @ " << __FILE__ << ":" << __LINE__

#define ELOG(...) LOG(dbslog::gLogger->error(__VA_ARGS__))
#define ILOG(...) LOG(dbslog::gLogger->info(__VA_ARGS__))
#define DLOG(...) LOG(dbslog::gLogger->debug(__VA_ARGS__))
#define TLOG(...) LOG(dbslog::gLogger->trace(__VA_ARGS__))
