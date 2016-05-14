//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "db/database.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include "gtest/intermittent.h"
#include <ostream>
#include <str>

TEST(DatabaseTest, VERYSLOW_open)
{
    const string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));
    ASSERT_BANNED(kDatabase, database.open(testDb));
}

TEST(DatabaseTest, VERYSLOW_put)
{
    const string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    ECode get(const string& key, string& value);

    string key("key");

    string value("value");
    ASSERT_OKAY(database.put(key, value));
    ASSERT_OKAY(database.put(key, value));

    string dbvalue;
    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value, dbvalue);

    string value2("value2");
    ASSERT_OKAY(database.put(key, value2));

    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value2, dbvalue);
}

TEST(DatabaseTest, VERYSLOW_get)
{
    const string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    string key("key");

    ASSERT_OKAY(database.erase(key));

    string dbvalue;
    ASSERT_BANNED(kNotFound, database.get(key, dbvalue));

    string value("value");
    ASSERT_OKAY(database.put(key, value));

    ASSERT_OKAY(database.get(key, dbvalue));
    ASSERT_EQ(value, dbvalue);
}

TEST(DatabaseTest, VERYSLOW_erase)
{
    const string& testDb = testing::gIntermittentDirectory + "rocksdb.tst";

    db::Database database;
    ASSERT_OKAY(database.open(testDb));

    string key("key");

    ASSERT_OKAY(database.erase(key));
    ASSERT_OKAY(database.erase(key));

    string value("value");
    ASSERT_OKAY(database.put(key, value));

    string dbvalue;
    ASSERT_OKAY(database.get(key, dbvalue));

    ASSERT_OKAY(database.erase(key));
    ASSERT_BANNED(kNotFound, database.get(key, dbvalue));
}
