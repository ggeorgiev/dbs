#include "parser/token.hpp"

#include "gtest/gtest.h"

TEST(TokenTest, types)
{
    auto type = parser::Token<int>::types(0, 'p');
    ASSERT_EQ(parser::TokenType::kPath, type & parser::TokenType::kPath);

    type = parser::Token<int>::types(10, 'p');
    ASSERT_EQ(parser::TokenType::kPath, type & parser::TokenType::kPath);
}
