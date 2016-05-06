//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "im/initialization_manager.hpp"
#include "gtest/framework.h"
#include <functional>
#include <memory>

class GlobalObject
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }
};

typedef std::shared_ptr<GlobalObject> GlobalObjectSPtr;
GlobalObjectSPtr gObjectSPtr = im::InitializationManager::subscribe(gObjectSPtr);

TEST(InitializationManagerTest, shared)
{
    ASSERT_NE(nullptr, gObjectSPtr);
}
