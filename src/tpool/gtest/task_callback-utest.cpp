//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_callback.h" // IWYU pragma: keep
#include "tpool/tpool.h"         // IWYU pragma: keep
#include "err/gtest/err.h"
#include "err/gtest/err_assert.h"
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <memory>

namespace tpool
{
class TaskCallbackTestTask : public Task
{
public:
    TaskCallbackTestTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHEnd;
    }
};

TEST(TaskCallbackTest, scheduled)
{
    auto pool = TPool::create(10);

    auto baseTask = std::make_shared<TaskCallbackTestTask>();
    pool->ensureScheduled(baseTask);

    TaskCallback::Function onFinish = [](const TaskSPtr&) -> ECode { EHEnd; };
    auto task = std::make_shared<TaskCallback>(0, baseTask, onFinish);

    pool->ensureScheduled(task);
    ASSERT_OKAY(task->join());
    pool->join();
}

} // namespace tpool
