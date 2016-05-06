//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "dp/memoization.hpp"
#include "dp/handle.hpp"
#include "dp/map_container.hpp"
#include "dp/solitary_container.hpp"
#include "gtest/framework.h"
#include "gtest/intermittent.h" // IWYU pragma: keep
#include <functional>
#include <memory>
#include <vector>

template <typename T>
class MemoizationTest : public ::testing::Test
{
public:
    typedef T Memoization;
};

typedef ::testing::Types<dp::Memoization<dp::SolitaryContainer, int>,
                         dp::Memoization<dp::MapContainer, int>>
    MemoizationType;

TYPED_TEST_CASE(MemoizationTest, MemoizationType);

static auto gFn5 = [](std::vector<dp::Handle::ControllerSPtr>&) { return 5; };
static auto gFn10 = [](std::vector<dp::Handle::ControllerSPtr>&) { return 10; };

TYPED_TEST(MemoizationTest, sanity)
{
    auto memoization = std::make_shared<typename TestFixture::Memoization>();
    auto handle = dp::Handle::create([memoization] { memoization->clear(); });

    auto value = memoization->get(handle, gFn10);
    ASSERT_EQ(10, value);

    // Calling second time should return the cached value. To be sure we using "wrong"
    // funtion
    value = memoization->get(handle, gFn5);
    ASSERT_EQ(10, value);

    handle = dp::Handle::create([memoization] { memoization->clear(); });

    // When the handle was reset the function should recompute the value.
    value = memoization->get(handle, gFn5);
    ASSERT_EQ(5, value);
}

TYPED_TEST(MemoizationTest, noDependencies)
{
    auto memoization = std::make_shared<typename TestFixture::Memoization>();
    auto handle = dp::Handle::create([memoization] { memoization->clear(); });

    auto value = memoization->get(handle, gFn10);
    ASSERT_EQ(10, value);
}

TYPED_TEST(MemoizationTest, dependencies)
{
    auto memoization = std::make_shared<typename TestFixture::Memoization>();
    auto handle = dp::Handle::create([memoization] { memoization->clear(); });

    auto depender = dp::Handle::create(nullptr);

    {
        auto fn10 = [depender](std::vector<dp::Handle::ControllerSPtr>& dependencies) {
            dependencies.push_back(depender->controller());
            return 10;
        };

        auto value = memoization->get(handle, fn10);
        ASSERT_EQ(10, value);
    }

    auto value = memoization->get(handle, gFn5);
    ASSERT_EQ(10, value);

    depender.reset();

    value = memoization->get(handle, gFn5);
    ASSERT_EQ(5, value);
}

TYPED_TEST(MemoizationTest, size)
{
    auto memoization = std::make_shared<typename TestFixture::Memoization>();

    ASSERT_EQ(0, memoization->size());

    {
        auto handle = dp::Handle::create([memoization] { memoization->clear(); });
        auto fn = [](std::vector<dp::Handle::ControllerSPtr>&) { return 10; };
        auto value = memoization->get(handle, fn);
        ASSERT_EQ(1, memoization->size());
    }

    ASSERT_EQ(0, memoization->size());
}

TEST(MapMemoizationTest, AdditinalArgs)
{
    auto memoization = std::make_shared<dp::Memoization<dp::MapContainer, int, int>>();
    auto handle = dp::Handle::create([memoization] { memoization->clear(); });

    auto fn = [](int a, std::vector<dp::Handle::ControllerSPtr>&) { return a; };

    auto value = memoization->get(handle, 10, fn);
    ASSERT_EQ(10, value);

    value = memoization->get(handle, 10, fn);
    ASSERT_EQ(10, value);

    value = memoization->get(handle, 5, fn);
    ASSERT_EQ(5, value);
}
