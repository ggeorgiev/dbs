//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/test_resource.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

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

    uint64_t crc = 5;
    auto value = std::make_shared<doim::DbValue>(crc);
    auto task = std::make_shared<task::DbPutTask>(key, value);

    ASSERT_OKAY((*task)());
}
