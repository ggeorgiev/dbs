//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "log/log.h"

namespace dbslog
{
static bool init()
{
    gLogger = spdlog::stdout_logger_mt("dbs");
    gLogger->set_pattern("%H:%M:%S.%e [%t] %v");
    return true;
}

DbsLoggerSPtr gLogger =
    im::InitializationManager::subscribe<DbsLoggerSPtr>(initialization_rank(), &init, [] {
        gLogger->flush();
        gLogger.reset();
    });

} // namespace dbslog
