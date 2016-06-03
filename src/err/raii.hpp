//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//
//
// Err is an error handling system that allows call stack tracking.
//

#pragma once

typedef std::function<void()> DeferFn;

class Defer
{
public:
    Defer(const DeferFn& func)
        : mFunc(func)
    {
    }

    ~Defer()
    {
        mFunc();
    }

private:
    DeferFn mFunc;
};