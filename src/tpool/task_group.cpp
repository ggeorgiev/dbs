//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_group.h"
#include "tpool/tpool.h"
#include <sstream>

namespace tpool
{
TaskGroup::TaskGroup(const TPoolSPtr& pool,
                     int priority,
                     const std::vector<TaskSPtr>& tasks)
    : Task(priority)
    , mPool(pool)
    , mTasks(tasks)
{
    for (const auto& task : mTasks)
        ASSERT(task != nullptr);
}

TaskGroup::~TaskGroup()
{
}

ECode TaskGroup::operator()()
{
    {
        TPoolSPtr pool = mPool.lock();
        ASSERT(pool != nullptr);

        for (const auto& task : mTasks)
            pool->ensureScheduled(task);
    }

    auto tasks = mTasks;
    while (true)
    {
        size_t i = 0;
        while (i < tasks.size())
        {
            if (!tasks[i]->finished())
            {
                ++i;
                continue;
            }

            tasks[i] = tasks.back();
            tasks.pop_back();
        }
        if (tasks.empty())
            break;

        tasks.front()->join();
    }

    EHEnd;
}

} // namespace tpool
