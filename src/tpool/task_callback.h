//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include <functional>
#include <shared_ptr>

namespace tpool
{
class TaskCallback;

typedef std::shared_ptr<TaskCallback> TaskCallbackSPtr;

class TaskCallback : public enable_make_shared<TaskCallback>, public Task
{
public:
    typedef std::function<ECode(const TaskSPtr&)> Function;

    TaskCallback(int priority,
                 const TaskSPtr& task,
                 const Function& onFinish,
                 const Function& onError);
    virtual ~TaskCallback();

    ECode operator()() override;

private:
    TaskSPtr mTask;
    Function mOnFinish;
    Function mOnError;
};

} // namespace tpool
