//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_group.h"
#include "tpool/task.h"
#include "tpool/tpool.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "err/gtest/err_assert.h"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <vector>

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

class TaskGroupTestFailTask : public Task
{
public:
    TaskGroupTestFailTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHBan(kExpected);
    }
};

TEST(TaskGroupTest, SLOW_scheduled)
{
    auto pool = TPool::create(10);

    auto task1 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task1);
    auto task2 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task2);
    auto task3 = std::make_shared<TaskGroupTestTask>();
    pool->ensureScheduled(task3);

    auto task = TaskGroup::make(pool, 0, std::vector<TaskSPtr>{task1, task2, task3});

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

    auto task = TaskGroup::make(pool, 0, std::vector<TaskSPtr>{task1, task2, task3});

    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

TEST(TaskGroupTest, assertNullSubtask)
{
    auto pool = TPool::create(10);
    ASSERT_ASSERT(TaskGroup::make(pool, 0, std::vector<TaskSPtr>{nullptr}));
}

TEST(TaskGroupTest, subtaskReturnsError)
{
    auto pool = TPool::create(10);

    auto task1 = std::make_shared<TaskGroupTestTask>();
    auto task2 = std::make_shared<TaskGroupTestFailTask>();
    auto task3 = std::make_shared<TaskGroupTestTask>();

    auto task = TaskGroup::make(pool, 0, std::vector<TaskSPtr>{task1, task2, task3});

    pool->ensureScheduled(task);

    ASSERT_BANNED(kExpected, task->join());
    pool->join();
}

} // namespace tpool
