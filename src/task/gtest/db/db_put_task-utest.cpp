//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "doim/db/db_key.h"
#include "doim/db/db_value.hpp"

#include "err/err.h"
#include "err/gtest/err.h"
#include <gtest/gtest.h>
#include <memory>

class DbPutTaskTest : public ::testing::Test
{
public:
    DbPutTaskTest()
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};

TEST_F(DbPutTaskTest, simple)
{
    auto key = doim::DbKey::unique(nullptr, "key");

    int crc = 5;
    auto value = doim::DbValue::make(crc);
    auto task = task::DbPutTask::make(key, value);

    ASSERT_OKAY((*task)());
}
