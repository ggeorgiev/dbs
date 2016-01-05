//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>

#include "db/database.h"
#include "err/err.h"
#include "gtest/err.h"
#include "gtest/intermittent.h"

TEST(DatabaseTest, SLOW_OPEN)
{
    const std::string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));
    ASSERT_BANNED(kDatabase, database.open(testDb));
}
