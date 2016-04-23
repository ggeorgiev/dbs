//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_callback.h"
#include <sstream>

namespace tpool
{
TaskCallback::TaskCallback(int priority, const TaskSPtr& task, const Function& onFinish)
    : Task(priority)
    , mTask(task)
    , mOnFinish(onFinish)
{
    ASSERT(mTask != nullptr);
    ASSERT(mOnFinish != nullptr);
}

TaskCallback::~TaskCallback()
{
}

ECode TaskCallback::operator()()
{
    EHTest(mTask->join());
    EHTest(mOnFinish(mTask));
    EHEnd;
}

} // namespace tpool
