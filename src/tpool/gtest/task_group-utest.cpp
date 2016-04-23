//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_group.h" // IWYU pragma: keep
#include "tpool/tpool.h"      // IWYU pragma: keep
#include "err/gtest/err.h"
#include "err/gtest/err_assert.h"
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <memory>

namespace tpool
{
class TaskGroupTestTask : public Task
{
public:
    TaskGroupTestTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHEnd;
    }
};

TEST(TaskGroupTest, scheduled)
{
    auto pool = TPool::create(10);

    auto task1 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task1);
    auto task2 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task2);
    auto task3 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task3);

    auto task =
        std::make_shared<TaskGroup>(pool, 0, std::vector<TaskSPtr>{task1, task2, task3});

    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

TEST(TaskGroupTest, notScheduled)
{
    auto pool = TPool::create(10);

    auto task1 = std::make_shared<TaskGroupTestTask>();
    auto task2 = std::make_shared<TaskGroupTestTask>();
    auto task3 = std::make_shared<TaskGroupTestTask>();

    auto task =
        std::make_shared<TaskGroup>(pool, 0, std::vector<TaskSPtr>{task1, task2, task3});

    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

TEST(TaskGroupTest, assertNullSubtask)
{
    auto pool = TPool::create(10);
    ASSERT_ASSERT(std::make_shared<TaskGroup>(pool, 0, std::vector<TaskSPtr>{nullptr}));
}

} // namespace tpool
