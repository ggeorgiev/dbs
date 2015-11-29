//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>

namespace parser
{
enum class Keyword
{
    kNil = 0,

    kCppProgram = 1 << 0,

    kAll = (1 << 3) - 1,
};

inline constexpr Keyword operator~(Keyword t1)
{
    return static_cast<Keyword>(~static_cast<int>(t1));
}

inline constexpr Keyword operator&(Keyword t1, Keyword t2)
{
    return static_cast<Keyword>(static_cast<int>(t1) & static_cast<int>(t2));
}

inline Keyword& operator&=(Keyword& t1, Keyword t2)
{
    t1 = t1 & t2;
    return t1;
}

inline constexpr Keyword operator|(Keyword t1, Keyword t2)
{
    return static_cast<Keyword>(static_cast<int>(t1) | static_cast<int>(t2));
}

inline Keyword& operator|=(Keyword& t1, Keyword t2)
{
    t1 = t1 | t2;
    return t1;
}

inline constexpr bool operator&&(Keyword t1, Keyword t2)
{
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}
}
