//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>

enum class TokenType
{
    kNil = 0,

    kWhiteSpace = 1 << 0,
    kPath = 1 << 1,

    kAll = (1 << 2) - 1,
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
