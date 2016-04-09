//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "tpool/task.h" // IWYU pragma: keep
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <memory>

namespace tpool
{
class TPool;
} // namespace tpool

template <typename T>
class TPoolTest : public ::testing::Test
{
public:
    typedef T TPool;
    typedef std::shared_ptr<TPool> TPoolSPtr;
};

typedef ::testing::Types<tpool::TPool> TPoolType;

TYPED_TEST_CASE(TPoolTest, TPoolType);
