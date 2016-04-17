//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/priority.hpp"
#include "err/err.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <atomic>
#include <future>
#include <iosfwd>
#include <memory>

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
typedef std::shared_ptr<Task> TaskSPtr;

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

    ECode join();

    // Returns the task priority.
    int priority();

    // Changes a task priority. Returns if this actually affected its value, because
    // the
    // priority of the task is also controlled from its dependees.
    bool updatePriority(int priority);

    // Handle of the task in the thread pool.
    Heap::handle_type& handle();

protected:
    void run();
    virtual void run(std::unique_lock<std::mutex>& lock);

    enum class State
    {
        kConstructed,
        kScheduled,
        kStarted,
        kFinished,
    };

    friend class TaskGroup;
    friend class TaskSequence;
    State escalateState(State newState);

    err::ErrorUPtr mExecutionError;

private:
    friend class TPool;
    void setHandle(Heap::handle_type&& handle)
    {
        mHandle = handle;
    }

    Heap::handle_type mHandle;
    PrioritySPtr mPriority;

    std::mutex mStateMutex;
    std::condition_variable mStateCondition;
    std::atomic<State> mState;
};

} // namespace tpool
