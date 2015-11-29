//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>

namespace parser
{
inline std::string colon()
{
    return ":";
}

enum class Operator
{
    kNil = 0,

    kColon = 1 << 0,

    kAll = (1 << 1) - 1,
};

inline constexpr Operator operator~(Operator t1)
{
    return static_cast<Operator>(~static_cast<int>(t1));
}

inline constexpr Operator operator&(Operator t1, Operator t2)
{
    return static_cast<Operator>(static_cast<int>(t1) & static_cast<int>(t2));
}

inline Operator& operator&=(Operator& t1, Operator t2)
{
    t1 = t1 & t2;
    return t1;
}

inline constexpr Operator operator|(Operator t1, Operator t2)
{
    return static_cast<Operator>(static_cast<int>(t1) | static_cast<int>(t2));
}

inline Operator& operator|=(Operator& t1, Operator t2)
{
    t1 = t1 | t2;
    return t1;
}

inline constexpr bool operator&&(Operator t1, Operator t2)
{
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}
}
