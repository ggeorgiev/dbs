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

    for (const auto& task : mTasks)
        EHTest(task->join());

    EHEnd;
}

} // namespace tpool
