//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_sequence.h"
#include <sstream>

namespace tpool
{
TaskSequence::TaskSequence(int priority, const std::vector<TaskSPtr>& tasks)
    : Task(priority)
    , mTasks(tasks)
{
}

TaskSequence::~TaskSequence()
{
}

ECode TaskSequence::operator()()
{
    for (const auto& task : mTasks)
    {
        if (task->escalateState(Task::State::kScheduled) != Task::State::kConstructed)
            if (task != *mTasks.begin())
                EHBan(kUnexpected);

        EHTest(task->join());
    }
    EHEnd;
}

} // namespace tpool
