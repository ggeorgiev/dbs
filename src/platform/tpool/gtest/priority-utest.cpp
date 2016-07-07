//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tpool/priority.hpp"
#include <gtest/gtest.h>
#include <iosfwd>
#include <limits>
#include <memory>

TEST(PriorityTest, construct)
{
    tpool::Priority priority1;
    ASSERT_EQ(std::numeric_limits<int>::max(), priority1.value());

    tpool::Priority priority2(5);
    ASSERT_EQ(5, priority2.value());
}

TEST(PriorityTest, updateValue)
{
    auto priority = std::make_shared<tpool::Priority>();
    ASSERT_TRUE(priority->update(5));
    ASSERT_EQ(5, priority->value());

    ASSERT_TRUE(priority->update(10));
    ASSERT_EQ(10, priority->value());

    ASSERT_TRUE(priority->update(5));
    ASSERT_EQ(5, priority->value());
}

TEST(PriorityTest, updateDependee)
{
    auto priority = std::make_shared<tpool::Priority>();

    auto dependee = std::make_shared<tpool::Priority>(5);
    ASSERT_TRUE(priority->update(dependee));
    ASSERT_EQ(4, priority->value());

    ASSERT_TRUE(dependee->update(10));
    ASSERT_TRUE(priority->update(dependee));
    ASSERT_EQ(9, priority->value());

    ASSERT_TRUE(dependee->update(5));
    ASSERT_TRUE(priority->update(dependee));
    ASSERT_EQ(4, priority->value());
}

TEST(PriorityTest, updateDependees)
{
    auto priority = std::make_shared<tpool::Priority>();

    auto dependee1 = std::make_shared<tpool::Priority>(1);
    auto dependee2 = std::make_shared<tpool::Priority>(2);
    ASSERT_TRUE(priority->update(dependee1));
    ASSERT_FALSE(priority->update(dependee2));
    ASSERT_EQ(0, priority->value());

    priority = std::make_shared<tpool::Priority>();
    ASSERT_TRUE(priority->update(dependee2));
    ASSERT_TRUE(priority->update(dependee1));
    ASSERT_EQ(0, priority->value());
}

TEST(PriorityTest, updateValueWithDependee)
{
    auto priority = std::make_shared<tpool::Priority>(0);
    auto dependee2 = std::make_shared<tpool::Priority>(2);
    ASSERT_FALSE(priority->update(dependee2));
    ASSERT_EQ(0, priority->value());

    priority = std::make_shared<tpool::Priority>(2);
    ASSERT_TRUE(priority->update(dependee2));
    ASSERT_EQ(1, priority->value());
}

TEST(PriorityTest, updateDependeeWithValue)
{
    auto dependee2 = std::make_shared<tpool::Priority>(2);

    auto priority = std::make_shared<tpool::Priority>();
    ASSERT_TRUE(priority->update(dependee2));
    ASSERT_TRUE(priority->update(0));
    ASSERT_EQ(0, priority->value());

    priority = std::make_shared<tpool::Priority>();
    ASSERT_TRUE(priority->update(dependee2));
    ASSERT_FALSE(priority->update(2));
    ASSERT_EQ(1, priority->value());
}
