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

    struct Hasher
    {
        std::size_t operator()(const TaskSequenceSPtr& sequence) const
        {
            std::size_t seed = 0;
            for (const auto& task : sequence->mTasks)
                boost::hash_combine(seed, mTaskHasher(task));
            return seed;
        }

        bool operator()(const TaskSequenceSPtr& sequence1,
                        const TaskSequenceSPtr& sequence2) const
        {
            return sequence1->mTasks == sequence2->mTasks;
        }

    private:
        std::hash<TaskSPtr> mTaskHasher;
    };

private:
    std::vector<TaskSPtr> mTasks;
};

} // namespace tpool
