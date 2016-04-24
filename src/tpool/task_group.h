//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <atomic>
#include <future>
#include <iosfwd>
#include <memory>

namespace tpool
{
class TPool;
typedef std::shared_ptr<TPool> TPoolSPtr;
typedef std::weak_ptr<TPool> TPoolWPtr;

class TaskGroup;
typedef std::shared_ptr<TaskGroup> TaskGroupSPtr;

class TaskGroup : public Task
{
public:
    TaskGroup(const TPoolSPtr& pool, int priority, const std::vector<TaskSPtr>& tasks);
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