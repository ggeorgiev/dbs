//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
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
    auto key = std::make_shared<doim::DbKey>("key");
    key = doim::gManager->unique(key);

    int crc = 5;
    auto value = std::make_shared<doim::DbValue>(crc);
    auto task = std::make_shared<task::DbPutTask>(key, value);

    ASSERT_OKAY((*task)());
}
