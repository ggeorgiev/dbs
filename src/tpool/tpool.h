//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "err/err.h"
#include <mutex> // IWYU pragma: keep
#include <atomic>
#include <future>
#include <memory>
#include <thread>
#include <stddef.h>

namespace tpool
{
class TPool : public std::enable_shared_from_this<TPool>
{
public:
    typedef std::thread Thread;
    typedef std::shared_ptr<Thread> ThreadSPtr;

    typedef std::future<ECode> Future;

    TPool(size_t maxThreads);

    void schedule(const TaskSPtr& task);
    void updatePriority(const TaskSPtr& task);
    void run();

    // Joins the pool. Waits until all the tasks current and new during the wait are
    // processed.
    void join();

private:
    size_t mMaxThreads;

    std::mutex mThreadsMutex;
    std::condition_variable mThreadsCondition;
    std::atomic<size_t> mFreeThreads;
    std::atomic<size_t> mThreads;

    std::mutex mTasksMutex;
    std::condition_variable mTasksCondition;
    Task::Heap mTasks;
    std::atomic<bool> mJoined;
};

} // namespace tpool
