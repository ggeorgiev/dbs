//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_group.h"
#include "tpool/tpool.h"
#include "err/err_assert.h"
#include <stddef.h>

namespace tpool
{
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
            if (!tasks[i]->conjoin() && !tasks[i]->finished())
            {
                ++i;
                continue;
            }

            EHTest(tasks[i]->reportError());
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
