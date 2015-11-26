//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>

template <typename C, typename T = std::char_traits<C>>
class StringStream
{
public:
    typedef C Char;
    typedef T CharTraits;

private:
    std::basic_string<Char, CharTraits> mString;
};
