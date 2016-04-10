//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "tpool/tpool.h"
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

void TPool::schedule(const TaskSPtr& task)
{
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        task->setHandle(mTasks.push(task));
    }

    if (mFreeThreads == 0)
    {
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

template <class T>
class reverse_lock
{
public:
    reverse_lock(T& lock)
        : mLock(lock)
    {
        mLock.unlock();
    }
    ~reverse_lock()
    {
        mLock.lock();
    }
    reverse_lock(const reverse_lock&) = delete;
    reverse_lock& operator=(const reverse_lock&) = delete;

private:
    T& mLock;
};

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

        ++mFreeThreads;

        bNeeded = mTasksCondition.wait_for(lock, std::chrono::milliseconds(2000), [this] {
            return !this->mTasks.empty() || this->mJoined;
        });

        --mFreeThreads;

        bNeeded = bNeeded && !this->mTasks.empty();
    }

    --mThreads;
    mThreadsCondition.notify_all();
}

// Joins the pool. Waits until all the tasks current and new during the wait are
// processed.
void TPool::join()
{
    mJoined = true;

    std::unique_lock<std::mutex> lock(mThreadsMutex);
    mThreadsCondition.wait(lock, [this] { return this->mThreads == 0; });
}

} // namespace tpool
