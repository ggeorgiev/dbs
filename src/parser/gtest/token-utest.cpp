#include "parser/token.hpp"

#include "gtest/gtest.h"

TEST(TokenTest, types)
{
    auto type = Token<int>::types(0, 'p');
    ASSERT_EQ(TokenType::kPath, type & TokenType::kPath);

    type = Token<int>::types(10, 'p');
    ASSERT_EQ(TokenType::kPath, type & TokenType::kPath);
}
