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
class TaskSequence;
typedef std::shared_ptr<TaskSequence> TaskSequenceSPtr;

class TaskSequence : public Task
{
public:
    TaskSequence(int priority, const std::vector<TaskSPtr>& tasks);
    virtual ~TaskSequence();

    ECode operator()() override;

private:
    std::vector<TaskSPtr> mTasks;
};

} // namespace tpool
