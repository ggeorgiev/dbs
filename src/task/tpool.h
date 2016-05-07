//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/tpool.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace task
{
class TPool;

typedef std::shared_ptr<TPool> TPoolSPtr;

class TPool : public tpool::TPool
{
public:
    TPool()
        : tpool::TPool(10)
    {
    }

    static constexpr int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }
};

extern TPoolSPtr gTPool;
}
