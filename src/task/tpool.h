//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/tpool.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace task
{
class TPool;

typedef shared_ptr<TPool> TPoolSPtr;

class TPool : public tpool::TPool
{
public:
    TPool()
        : tpool::TPool(10)
    {
    }

    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }
};

extern TPoolSPtr gTPool;
}
