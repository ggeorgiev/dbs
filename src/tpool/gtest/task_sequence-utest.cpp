//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_sequence.h"
#include "tpool/task.h"
#include "tpool/tpool.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "gtest/framework.h"
#include <memory>
#include <vector>

namespace tpool
{
class TaskSequenceTestTask : public Task
{
public:
    TaskSequenceTestTask()
        : Task(0)
    {
    }

    ECode operator()() override
    {
        EHEnd;
    }
};

TEST(TaskSequenceTest, sanity)
{
    auto pool = TPool::create(10);

    auto task1 = std::make_shared<TaskSequenceTestTask>();
    auto task2 = std::make_shared<TaskSequenceTestTask>();
    auto task3 = std::make_shared<TaskSequenceTestTask>();

    auto task =
        std::make_shared<TaskSequence>(0, std::vector<TaskSPtr>{task1, task2, task3});

    pool->ensureScheduled(task);

    ASSERT_OKAY(task->join());
    pool->join();
}

} // namespace tpool
