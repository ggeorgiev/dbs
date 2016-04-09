//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "tpool/task.h"

namespace tpool
{
bool Task::Compare::operator()(const TaskSPtr& task1, const TaskSPtr& task2) const
{
    return task1->priority() > task2->priority();
}

Task::Task(int priority)
    : mPriority(std::make_shared<Priority>(priority))
    , mFinished(false)
{
}

Task::~Task()
{
}

ECode Task::run()
{
    ILOG("[RUN   ] " + description());
    ECode code = (*this)();

    ILOG("[  DONE] " + description());
    mFinished = true;

    EHTest(code);
    EHEnd;
}

ECode Task::join()
{
    if (!mFinished)
        EHTest(run());
    EHEnd;
}

int Task::priority()
{
    return mPriority->value();
}

bool Task::updatePriority(int priority)
{
    return mPriority->update(priority);
}

Task::Heap::handle_type& Task::handle()
{
    return mHandle;
}

} // namespace tpool
