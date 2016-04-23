//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tpool/reverse_lock.hpp"
#include "tpool/tpool.h"
#include "log/log.h"
#include <chrono>
#include <cstring>
#include <memory>
#include <ratio>
#include <thread>

namespace tpool
{
TPool::TPool(size_t maxThreads)
    : mMaxThreads(maxThreads)
    , mFreeThreads(0)
    , mThreads(0)
    , mJoined(false)
{
}

TPoolSPtr TPool::create(size_t maxThreads)
{
    // Thread pools are created rarely. No need of any tricks to allow make_shared
    // for a protected method.
    return TPoolSPtr(new TPool(maxThreads));
}

void TPool::ensureScheduled(const TaskSPtr& task)
{
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        if (task->markAsScheduled())
            return;
        task->setHandle(mTasks.push(task));
    }

    if (mFreeThreads == 0)
    {
        std::unique_lock<std::mutex> lock(mThreadsMutex);
        if (mThreads < mMaxThreads)
        {
            ++mThreads;
            std::thread thread(&TPool::run, shared_from_this());
            thread.detach();
        }
    }
    else
    {
        mTasksCondition.notify_one();
    }
}

void TPool::updatePriority(const TaskSPtr& task)
{
    mTasks.update(task->handle());
}

void TPool::run()
{
    bool bNeeded = true;
    while (bNeeded)
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        while (!mTasks.empty())
        {
            auto task = mTasks.top();
            mTasks.pop();

            reverse_lock<std::unique_lock<std::mutex>> un(lock);
            task->run();
        }

        if (mJoined)
            break;

        ++mFreeThreads;

        bNeeded = mTasksCondition.wait_for(lock, std::chrono::seconds(2)) ==
                  std::cv_status::no_timeout;

        --mFreeThreads;
    }

    std::unique_lock<std::mutex> lock(mThreadsMutex);
    --mThreads;
    mThreadsCondition.notify_all();
}

// Joins the pool. Waits until all the tasks current and new during the wait are
// processed.
void TPool::join()
{
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        mJoined = true;
        mTasksCondition.notify_all();
    }

    std::unique_lock<std::mutex> lock(mThreadsMutex);
    mThreadsCondition.wait(lock, [this] { return this->mThreads == 0; });
}

} // namespace tpool
