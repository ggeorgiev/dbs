//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "tpool/task.hpp" // IWYU pragma: keep
#include "tpool/tpool.hpp"
#include "err/err.h"
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <memory>

template <typename T>
class TPoolTest : public ::testing::Test
{
public:
    typedef T TPool;
    typedef std::shared_ptr<TPool> TPoolSPtr;
};

typedef ::testing::Types<tpool::TPool> TPoolType;

TYPED_TEST_CASE(TPoolTest, TPoolType);

TYPED_TEST(TPoolTest, SLOW_initialize)
{
    std::srand(std::time(0));

    auto pool = std::make_shared<tpool::TPool>(100);
    for (int i = 0; i < 10; ++i)
    {
        int priority = std::rand();
        auto task = std::make_shared<tpool::Task>(priority, [pool, priority]() -> ECode {
            for (int j = 0; j < 10; ++j)
            {
                auto task =
                    std::make_shared<tpool::Task>(priority,
                                                  [priority]() -> ECode { EHEnd; });
                pool->schedule(task);
            }
            EHEnd;
        });
        pool->schedule(task);
    }
    pool->join();
}
