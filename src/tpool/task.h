//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/priority.hpp"
#include "err/err.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace boost
{
namespace heap
{
template <class T>
struct compare;
} // namespace heap
} // namespace boost

namespace tpool
{
class Task;

typedef shared_ptr<Task> TaskSPtr;

class Task
{
public:
    struct Compare
    {
        bool operator()(const TaskSPtr& task1, const TaskSPtr& task2) const;
    };
    typedef boost::heap::fibonacci_heap<TaskSPtr, boost::heap::compare<Compare>> Heap;

    Task(int priority);
    virtual ~Task();

    virtual ECode operator()() = 0;

    // Joins the current thread with the task until it is finished. It expects the task to
    // be already sheduled.
    ECode join();

    // Executed the task in the current thread. It expects the task to not be scheduled in
    // the thread pool.
    ECode execute();

    // Returns the same result that was recieved from the execution of the task.
    ECode reportError() const;

    // Returns the task priority.
    int priority();

    // Changes a task priority. Returns if this actually affected its value, because the
    // priority of the task is also controlled from its dependees.
    bool updatePriority(int priority);

    // Handle of the task in the thread pool.
    Heap::handle_type& handle();

protected:
    void run();
    void run(std::unique_lock<std::mutex>& lock);

    virtual void onStart() const;
    virtual void onFinish(ECode code) const;

    enum class EState
    {
        kConstructed,
        kScheduled,
        kStarted,
        kFinished,
    };

    friend class TaskCallback;
    friend class TaskGroup;
    friend class TaskSequence;

    bool markAsScheduled();
    bool finished() const;
    bool conjoin();

    err::ErrorUPtr mExecutionError;

private:
    friend class TPool;
    void setHandle(Heap::handle_type&& handle)
    {
        mHandle = handle;
    }

    Heap::handle_type mHandle;
    PrioritySPtr mPriority;

    mutable std::mutex mStateMutex;
    std::condition_variable mStateCondition;
    std::atomic<EState> mState;
};

} // namespace tpool
