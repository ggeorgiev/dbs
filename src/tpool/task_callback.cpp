//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tpool/task_callback.h"
#include "err/err_assert.h"
#include <sstream>

namespace tpool
{
TaskCallback::TaskCallback(int priority,
                           const TaskSPtr& task,
                           const Function& onFinish,
                           const Function& onError)
    : Task(priority)
    , mTask(task)
    , mOnFinish(onFinish)
    , mOnError(onError)
{
    ASSERT(mTask != nullptr);
    ASSERT(mOnFinish != nullptr || mOnError != nullptr);
}

TaskCallback::~TaskCallback()
{
}

ECode TaskCallback::operator()()
{
    (void)mTask->markAsScheduled();
    ECode code = mTask->join();

    if (code == err::kSuccess)
    {
        if (mOnFinish != nullptr)
            EHTest(mOnFinish(mTask));
    }
    else
    {
        if (mOnError == nullptr)
            EHTest(code);
        EHTest(mOnError(mTask));
    }
    EHEnd;
}

} // namespace tpool
