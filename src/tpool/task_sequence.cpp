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
    // To enforce a sequencial order we need to keep all the tasks out of the pool, except
    // the first one - the first one is OK to be already running.

    auto first = mTasks.front();
    (void)first->markAsScheduled();
    EHTest(first->join());

    for (size_t i = 1; i < mTasks.size(); ++i)
        EHTest(mTasks[i]->execute());

    EHEnd;
}

} // namespace tpool
