//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "im/initialization_manager.hpp"
#include <gtest/gtest.h>
#include <functional>
#include <memory>

class GlobalObject
{
public:
    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }
};

typedef shared_ptr<GlobalObject> GlobalObjectSPtr;
GlobalObjectSPtr gObjectSPtr = im::InitializationManager::subscribe(gObjectSPtr);

TEST(InitializationManagerTest, shared)
{
    ASSERT_NE(nullptr, gObjectSPtr);
}
