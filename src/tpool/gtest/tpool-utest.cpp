//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tpool/tpool.h" // IWYU pragma: keep
#include "tpool/task.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include <shared_ptr>

namespace tpool
{
template <typename T>
class TPoolTest : public ::testing::Test
{
public:
    typedef T TPool;
    typedef shared_ptr<TPool> TPoolSPtr;
};

typedef ::testing::Types<tpool::TPool> TPoolType;

TYPED_TEST_CASE(TPoolTest, TPoolType);

TYPED_TEST(TPoolTest, JoinEmpty)
{
    auto pool = TestFixture::TPool::create(10);
    pool->join();
}

class TPoolTestTask : public Task
{
public:
    TPoolTestTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHEnd;
    }
};

TYPED_TEST(TPoolTest, ensureScheduled)
{
    auto pool = TestFixture::TPool::create(10);

    auto task = std::make_shared<TPoolTestTask>();
    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

} // namespace tpool
