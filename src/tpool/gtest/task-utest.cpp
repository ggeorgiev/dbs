//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task.h"
#include "tpool/tpool.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include <gtest/gtest.h>
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
