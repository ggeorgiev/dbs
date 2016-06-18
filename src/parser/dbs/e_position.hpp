//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <axe.h> // IWYU pragma: keep

namespace parser
{
using namespace axe;
typedef const string::const_iterator I;

static auto r_endl = r_str("\r\n") | r_str("\n\r") | r_char('\n');
static auto r_space = r_char(' ') | r_char('\t');

struct Position
{
    void operator()(I& i1, I& i2)
    {
        lineIterator = i2;
        ++line;
    }

    size_t line;
    string::const_iterator lineIterator;
};
}
