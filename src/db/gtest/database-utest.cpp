//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "db/database.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/intermittent.h" // IWYU pragma: keep
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>

TEST(DatabaseTest, SLOW_open)
{
    const std::string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));
    ASSERT_BANNED(kDatabase, database.open(testDb));
}

TEST(DatabaseTest, SLOW_put)
{
    const std::string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    ECode get(const std::string& key, std::string& value);

    std::string key("key");

    std::string value("value");
    ASSERT_OKAY(database.put(key, value));
    ASSERT_OKAY(database.put(key, value));

    std::string dbvalue;
    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value, dbvalue);

    std::string value2("value2");
    ASSERT_OKAY(database.put(key, value2));

    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value2, dbvalue);
}

TEST(DatabaseTest, SLOW_get)
{
    const std::string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    std::string key("key");

    ASSERT_OKAY(database.erase(key));

    std::string dbvalue;
    ASSERT_BANNED(kNotFound, database.get(key, dbvalue));

    std::string value("value");
    ASSERT_OKAY(database.put(key, value));

    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value, dbvalue);
}

TEST(DatabaseTest, SLOW_erase)
{
    const std::string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    std::string key("key");

    ASSERT_OKAY(database.erase(key));
    ASSERT_OKAY(database.erase(key));

    std::string value("value");
    ASSERT_OKAY(database.put(key, value));

    std::string dbvalue;
    ASSERT_OKAY(database.get(key, dbvalue));

    ASSERT_OKAY(database.erase(key));
    ASSERT_BANNED(kNotFound, database.get(key, dbvalue));
}
