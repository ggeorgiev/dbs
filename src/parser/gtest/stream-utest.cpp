//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/string_stream.hpp"
#include "gtest/framework.h"

template <typename T>
class StreamTest : public ::testing::Test
{
public:
    typedef T Functor;
};

typedef ::testing::Types<parser::StringStream<char>> StreamType;

TYPED_TEST_CASE(StreamTest, StreamType);

TYPED_TEST(StreamTest, PERFORMANCE_CPU)
{
}
