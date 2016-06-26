//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_particle.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/url/url.h"
#include "err/err.h"

namespace parser
{
struct Url
{
    auto set()
    {
        return e_ref([this](I& i1, I& i2) { mUrl = doim::Url::unique(string(i1, i2)); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & (r_ident() & r_colon & r_slash & r_slash & r_path) >> set();
    }

    doim::UrlSPtr mUrl;
};
}
