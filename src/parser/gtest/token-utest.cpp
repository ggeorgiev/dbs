#include "parser/token.hpp"
#include "parser/token_type.hpp"

#include <gtest/gtest.h>

TEST(TokenTest, types)
{
    auto type = parser::Token<int>::types('p');
    ASSERT_EQ(parser::TokenType::kPath, type & parser::TokenType::kPath);
}
