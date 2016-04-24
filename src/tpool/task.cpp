//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tpool/reverse_lock.hpp"
#include "tpool/task.h"
#include <sstream>

namespace tpool
{
bool Task::Compare::operator()(const TaskSPtr& task1, const TaskSPtr& task2) const
{
    return task1->priority() > task2->priority();
}

Task::Task(int priority)
    : mPriority(std::make_shared<Priority>(priority))
    , mState(State::kConstructed)
{
}

Task::~Task()
{
}

bool Task::markAsScheduled()
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    if (mState != State::kConstructed)
        return false;

    mState = State::kScheduled;
    mStateCondition.notify_all();

    return true;
}

bool Task::finished() const
{
    std::unique_lock<std::mutex> lock(mStateMutex);
    return mState == State::kFinished;
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
    ASSERT(mState != State::kConstructed);
    if (mState != State::kScheduled)
        return;

    mState = State::kStarted;
    mStateCondition.notify_all();
    {
        reverse_lock<std::unique_lock<std::mutex>> un(lock);
        onStart();

        ECode code = (*this)();

        onFinish(code);

        if (code != err::kSuccess)
            mExecutionError.reset(err::gError.release());
    }
    mState = State::kFinished;
    mStateCondition.notify_all();
}

ECode Task::join()
{
    {
        std::unique_lock<std::mutex> lock(mStateMutex);
        ASSERT(this->mState == State::kScheduled || this->mState == State::kStarted ||
               this->mState == State::kFinished);

        if (mState == State::kScheduled)
            run(lock);

        mStateCondition.wait(lock, [this] { return this->mState == State::kFinished; });
    }
    EHTest(reportError());
    EHEnd;
}

ECode Task::execute()
{
    {
        std::unique_lock<std::mutex> lock(mStateMutex);
        ASSERT(this->mState == State::kConstructed);

        mState = State::kScheduled;
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