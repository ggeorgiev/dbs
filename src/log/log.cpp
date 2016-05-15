//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "log/log.h"
#include <functional>

namespace dbslog
{
static bool init()
{
    gLogger = spdlog::stdout_logger_mt("dbs");
    gLogger->set_level(spdlog::level::trace);
    gLogger->set_pattern("# %H:%M:%S.%e %L > %v");
    return true;
}

DbsLoggerSPtr gLogger =
    im::InitializationManager::subscribe<DbsLoggerSPtr>(rank(), &init, [] {
        gLogger->flush();
        gLogger.reset();
    });

} // namespace dbslog
