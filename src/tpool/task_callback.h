//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include <functional>
#include <memory>

namespace tpool
{
class TaskCallback;

typedef std::shared_ptr<TaskCallback> TaskCallbackSPtr;

class TaskCallback : public Task
{
public:
    template <typename... MakeArgs>
    static std::shared_ptr<TaskCallback> make(const MakeArgs&... args)
    {
        return std::make_shared<TaskCallback>(args...);
    }

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
