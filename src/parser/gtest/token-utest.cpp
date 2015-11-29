#include "parser/token.hpp"

#include <gtest/gtest.h>

typedef parser::Token<int> Token;

TEST(TokenTest, types)
{
    auto type = Token::typeBody(0, 'p');
    ASSERT_TRUE(type.test(Token::kPath));
}
