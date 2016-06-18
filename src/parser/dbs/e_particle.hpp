//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_position.hpp"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <axe.h> // IWYU pragma: keep

namespace parser
{
static auto r_colon = r_char(':');
static auto r_semicolon = r_char(';');

struct Particle
{
    template <typename T>
    static auto r_he(const T& r_ws)
    {
        return r_ws & r_colon;
    }

    template <typename T>
    static auto r_se(const T& r_ws)
    {
        return r_ws & r_semicolon;
    }
};
}
