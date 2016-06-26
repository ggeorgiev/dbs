//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "parser/dbs/e_url.hpp"
#include "dom/prj/depository.h"
#include "doim/cxx/cxx_framework.h"

namespace parser
{
static auto r_frameworkKw = r_str("cxx_framework");

struct CxxFramework
{
    auto set()
    {
        return e_ref([this](I& i1, I& i2) {
            mFramework = doim::CxxFramework::unique(string(i1, i2));
        });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_ident() >> set();
    }

    doim::CxxFrameworkSPtr mFramework;
};

struct CxxFrameworkSet
{
    CxxFrameworkSet()
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mFrameworks.clear(); });
    }

    auto insert()
    {
        return e_ref([this](I& i1, I& i2) { mFrameworks.insert(mFramework.mFramework); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_empty() >> reset() & *(mFramework.rule(r_ws) >> insert());
    }

    doim::CxxFrameworkSet mFrameworks;

    CxxFramework mFramework;
};
}
