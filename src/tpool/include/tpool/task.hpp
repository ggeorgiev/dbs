//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/priority.hpp"

#include "err/err.h"

#include <boost/heap/fibonacci_heap.hpp>

#include <thread>
#include <future>
#include <functional>

namespace tpool
{
class Task;
typedef std::shared_ptr<Task> TaskSPtr;

class Task
{
public:
    struct Priority
    {
        bool operator()(const TaskSPtr& task1, const TaskSPtr& task2) const
        {
            return task1->priority() > task2->priority();
        }
    };
    typedef boost::heap::fibonacci_heap<TaskSPtr, boost::heap::compare<Priority>> Heap;

    typedef std::function<ECode()> Function;
    typedef std::future<ECode> Future;

    Task(int priority, Function&& function)
        : mFunction(function)
    {
    }
    void run()
    {
        mFunction();
    }
    int priority()
    {
        return mPriority->value();
    }
    void setPriority(int priority)
    {
        // mPriority->update(priority);
    }
    Heap::handle_type& handle()
    {
        return mHandle;
    }
    void setHandle(Heap::handle_type&& handle)
    {
        mHandle = handle;
    }

private:
    Heap::handle_type mHandle;
    PrioritySPtr mPriority;
    Function mFunction;
};

typedef std::shared_ptr<Task> TaskSPtr;

} // namespace tpool
