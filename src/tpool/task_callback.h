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
class TaskCallback;
typedef std::shared_ptr<TaskCallback> TaskCallbackSPtr;

class TaskCallback : public Task
{
public:
    typedef std::function<ECode(const TaskSPtr&)> Function;

    TaskCallback(int priority, const TaskSPtr& task, const Function& onFinish);
    virtual ~TaskCallback();

    ECode operator()() override;

private:
    TaskSPtr mTask;
    Function mOnFinish;
};

} // namespace tpool
