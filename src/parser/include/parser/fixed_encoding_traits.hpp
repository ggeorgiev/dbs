//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <cstddef>

namespace parser
{
template <typename C1, typename C2 = C1>
class fixed_encoding_traits
{
public:
    typedef C1 Char;
    typedef C2 Code;

    static size_t sequence_length(const Char* position) { return 1U; }
    static Code to_code(const Char* position) { return *position; }
};
}
