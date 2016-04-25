//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dp/memoization.hpp"
#include "gtest/intermittent.h" // IWYU pragma: keep
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <string>

TEST(MemoizationTest, noAdditinalArgs)
{
    auto memoization = std::make_shared<dp::Memoization<int>>();
    auto handle = dp::Memoization<int>::Handle::create(memoization);

    auto value = memoization->get(handle->controller());
    ASSERT_EQ(0, value);

    memoization->put(handle->controller(), 10);

    value = memoization->get(handle->controller());
    ASSERT_EQ(10, value);
}

TEST(MemoizationTest, additinalArgs)
{
    auto memoization = std::make_shared<dp::Memoization<int, int>>();
    auto handle = dp::Memoization<int, int>::Handle::create(memoization);

    auto value = memoization->get({handle->controller(), 5});
    ASSERT_EQ(0, value);

    memoization->put({handle->controller(), 5}, 10);

    value = memoization->get({handle->controller(), 5});
    ASSERT_EQ(10, value);
}

TEST(MemoizationTest, size)
{
    auto memoization = std::make_shared<dp::Memoization<int>>();

    ASSERT_EQ(0, memoization->size());

    {
        auto handle = dp::Memoization<int>::Handle::create(memoization);
        memoization->put(handle->controller(), 10);
        ASSERT_EQ(1, memoization->size());
    }

    ASSERT_EQ(0, memoization->size());
}
