//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>

namespace parser
{
enum class TokenType
{
    kNil = 0,

    kKeyword = 1 << 0,
    kPath = 1 << 1,
    kWhiteSpace = 1 << 2,

    kAll = (1 << 3) - 1,
};

inline constexpr TokenType operator~(TokenType t1)
{
    return static_cast<TokenType>(~static_cast<int>(t1));
}

inline constexpr TokenType operator&(TokenType t1, TokenType t2)
{
    return static_cast<TokenType>(static_cast<int>(t1) & static_cast<int>(t2));
}

inline TokenType& operator&=(TokenType& t1, TokenType t2)
{
    t1 = t1 & t2;
    return t1;
}

inline constexpr TokenType operator|(TokenType t1, TokenType t2)
{
    return static_cast<TokenType>(static_cast<int>(t1) | static_cast<int>(t2));
}

inline TokenType& operator|=(TokenType& t1, TokenType t2)
{
    t1 = t1 | t2;
    return t1;
}
}
