//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

namespace tpool
{
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
} // namespace tpool
