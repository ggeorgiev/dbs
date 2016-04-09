//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/priority.hpp"
#include "err/err.h"
#include "log/log.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <atomic>
#include <functional>
#include <future>
#include <thread>

namespace tpool
{
class Task;
typedef std::shared_ptr<Task> TaskSPtr;

class Task
{
public:
    struct Compare
    {
        bool operator()(const TaskSPtr& task1, const TaskSPtr& task2) const
        {
            return task1->priority() > task2->priority();
        }
    };
    typedef boost::heap::fibonacci_heap<TaskSPtr, boost::heap::compare<Compare>> Heap;

    typedef std::future<ECode> Future;

    Task(int priority)
        : mPriority(std::make_shared<Priority>(priority))
        , mFinished(false)
    {
    }

    virtual ~Task(){};

    virtual ECode operator()()
    {
        EHEnd;
    };

    virtual std::string description() const = 0;

    ECode run()
    {
        ILOG("[RUN   ] " + description());
        ECode code = (*this)();
        ILOG("[  DONE] " + description());
        mFinished = true;
        EHTest(code);
        EHEnd;
    }

    ECode join()
    {
        if (!mFinished)
            EHTest(run());
        EHEnd;
    }

    // Returns the task priority.
    int priority()
    {
        return mPriority->value();
    }

    // Changes a task priority. Returns if this actually affected its value, because the
    // priority of the task is also controlled from its dependees.
    bool updatePriority(int priority)
    {
        return mPriority->update(priority);
    }

    // Handle of the task in the thread pool.
    Heap::handle_type& handle()
    {
        return mHandle;
    }

private:
    friend class TPool;

    void setHandle(Heap::handle_type&& handle)
    {
        mHandle = handle;
    }

    Heap::handle_type mHandle;
    PrioritySPtr mPriority;

    std::atomic<bool> mFinished;
};

typedef std::shared_ptr<Task> TaskSPtr;

} // namespace tpool
