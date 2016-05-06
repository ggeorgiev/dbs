//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task.h"  // IWYU pragma: keep
#include "tpool/tpool.h" // IWYU pragma: keep
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include <memory>

namespace tpool
{
class TaskTestTask : public Task
{
public:
    TaskTestTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHEnd;
    }
};

TEST(TaskTest, doubleEnsureScheduled)
{
    auto pool = TPool::create(10);

    auto task = std::make_shared<TaskTestTask>();
    pool->ensureScheduled(task);
    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

} // namespace tpool
