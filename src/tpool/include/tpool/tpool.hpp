//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <tpool/task.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <vector>

template <class T>
class unlock_guard
{
public:
    unlock_guard(T& mutex) : mMutex(mutex) { mMutex.unlock(); }
    ~unlock_guard() { mMutex.lock(); }
    unlock_guard(const unlock_guard&) = delete;
    unlock_guard& operator=(const unlock_guard&) = delete;

private:
    T& mMutex;
};

namespace tpool
{
class TPool : public std::enable_shared_from_this<TPool>
{
public:
    typedef std::thread Thread;
    typedef std::shared_ptr<Thread> ThreadSPtr;

    typedef std::future<ECode> Future;

    TPool(size_t maxThreads)
        : mMaxThreads(maxThreads), mFreeThreads(0), mThreads(0), mJoined(false)
    {
    }
    void schedule(const TaskSPtr& task)
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
    void updatePriority(const TaskSPtr& task) { mTasks.update(task->handle()); }
    void run()
    {
        bool bNeeded = true;
        while (bNeeded)
        {
            std::unique_lock<std::mutex> lock(mTasksMutex);
            while (!mTasks.empty())
            {
                auto task = mTasks.top();
                mTasks.pop();

                unlock_guard<std::unique_lock<std::mutex>> un(lock);
                task->run();
            }

            ++mFreeThreads;

            bNeeded =
                mTasksCondition.wait_for(lock, std::chrono::milliseconds(2000), [this] {
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
    void join()
    {
        mJoined = true;

        std::unique_lock<std::mutex> lock(mThreadsMutex);
        mThreadsCondition.wait(lock, [this] { return this->mThreads == 0; });
    }

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
