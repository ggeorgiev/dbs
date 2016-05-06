//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/token.hpp"
#include "gtest/framework.h"
#include <bitset>
#include <iosfwd>

typedef parser::Token<int> Token;

TEST(TokenTest, types)
{
    auto type = Token::typeBody(typename Token::Type(), 0, 'p');
    ASSERT_TRUE(type.test(Token::kPath));
}
