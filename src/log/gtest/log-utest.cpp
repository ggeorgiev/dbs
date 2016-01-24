//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "log/log.h"
#include <gtest/gtest.h>

TEST(Log, foo)
{
    dbslog::gLogger->info() << "test";
    dbslog::gLogger->info("test");
}
