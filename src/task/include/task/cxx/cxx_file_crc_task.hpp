//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"

namespace task
{
class CxxFileCrcTask : public tpool::Task
{
    CxxFileCrcTask()
        : tpool::Task(0, nullptr)
    {
    }
};
}
