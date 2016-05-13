//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include <memory>
#include <vector>

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
    template <typename... MakeArgs>
    static std::shared_ptr<TaskGroup> make(const MakeArgs&... args)
    {
        return std::make_shared<TaskGroup>(args...);
    }

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
