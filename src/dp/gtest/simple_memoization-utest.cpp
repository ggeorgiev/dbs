//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "dp/simple_memoization.hpp"
#include "gtest/framework.h"
#include "gtest/intermittent.h" // IWYU pragma: keep
#include <functional>
#include <memory>

static auto gFn5 = []() { return 5; };
static auto gFn10 = []() { return 10; };

TEST(SimpleMemoizationTest, sanity)
{
    auto memoization = std::make_shared<dp::SimpleMemoization<int>>();

    auto value = memoization->get(gFn10);
    ASSERT_EQ(10, value);

    // Calling second time should return the cached value. To be sure we using "wrong"
    // funtion
    value = memoization->get(gFn5);
    ASSERT_EQ(10, value);

    memoization->clear();

    // When it was reset the function should recompute the value.
    value = memoization->get(gFn5);
    ASSERT_EQ(5, value);
}
