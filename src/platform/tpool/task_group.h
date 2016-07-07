//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include "err/err_assert.h"
#include <shared_ptr>
#include <vector>

namespace tpool
{
class TPool;

typedef shared_ptr<TPool> TPoolSPtr;
typedef weak_ptr<TPool> TPoolWPtr;

class TaskGroup;
typedef shared_ptr<TaskGroup> TaskGroupSPtr;

class TaskGroup : public enable_make_shared<TaskGroup>, public Task
{
public:
    template <typename T>
    TaskGroup(const TPoolSPtr& pool, int priority, const std::vector<T>& tasks)
        : Task(priority)
        , mPool(pool)
    {
        mTasks.reserve(tasks.size());
        for (const auto& task : tasks)
        {
            ASSERT(task != nullptr);
            mTasks.push_back(std::static_pointer_cast<tpool::Task>(task));
        }
    }
    virtual ~TaskGroup();

    ECode operator()() override;

    const std::vector<TaskSPtr>& tasks() const
    {
        return mTasks;
    }

private:
    TPoolWPtr mPool;
    std::vector<TaskSPtr> mTasks;
};

} // namespace tpool
