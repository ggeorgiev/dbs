//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tpool/task.h"
#include "tpool/reverse_lock.hpp"
#include "err/err_assert.h"

namespace tpool
{
bool Task::Compare::operator()(const TaskSPtr& task1, const TaskSPtr& task2) const
{
    return task1->priority() > task2->priority();
}

Task::Task(int priority)
    : mPriority(std::make_shared<Priority>(priority))
    , mState(EState::kConstructed)
{
}

Task::~Task()
{
}

bool Task::markAsScheduled()
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    if (mState != EState::kConstructed)
        return false;

    mState = EState::kScheduled;
    mStateCondition.notify_all();

    return true;
}

bool Task::finished() const
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    return mState == EState::kFinished;
}

bool Task::conjoin()
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    if (mState == EState::kScheduled)
    {
        run(lock);
        return true;
    }
    return false;
}

void Task::onStart() const
{
}

void Task::onFinish(ECode code) const
{
}

void Task::run()
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    run(lock);
}

void Task::run(std::unique_lock<std::mutex>& lock)
{
    ASSERT(mState != EState::kConstructed);
    if (mState != EState::kScheduled)
        return;

    mState = EState::kStarted;
    mStateCondition.notify_all();
    {
        reverse_lock<std::unique_lock<std::mutex>> un(lock);
        onStart();

        ECode code = (*this)();

        onFinish(code);

        if (code != err::kSuccess)
            mExecutionError.reset(err::gError.release());
    }
    mState = EState::kFinished;
    mStateCondition.notify_all();
}

ECode Task::join()
{
    {
        std::unique_lock<std::mutex> lock(mStateMutex);
        ASSERT(this->mState == EState::kScheduled || this->mState == EState::kStarted ||
               this->mState == EState::kFinished);

        if (mState == EState::kScheduled)
            run(lock);

        mStateCondition.wait(lock, [this] { return this->mState == EState::kFinished; });
    }
    EHTest(reportError());
    EHEnd;
}

ECode Task::execute()
{
    {
        std::unique_lock<std::mutex> lock(mStateMutex);
        ASSERT(this->mState == EState::kConstructed);

        mState = EState::kScheduled;
        run(lock);
    }
    EHTest(reportError());
    EHEnd;
}

int Task::priority()
{
    return mPriority->value();
}

bool Task::updatePriority(int priority)
{
    return mPriority->update(priority);
}

Task::Heap::handle_type& Task::handle()
{
    return mHandle;
}

ECode Task::reportError() const
{
    ASSERT(finished());

    if (mExecutionError == nullptr)
        EHEnd;

    err::ErrorRPtr error = new err::Error(*mExecutionError);
    err::gError.reset(error);
    return mExecutionError->code();
}

} // namespace tpool
