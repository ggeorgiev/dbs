//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_file_set.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "parser/dbs/e_url.hpp"
#include "dom/prj/depository.h"
#include "doim/cxx/cxx_framework.h"

namespace parser
{
static auto r_frameworkKw = r_str("cxx_framework");

struct Framework
{
    Framework()
    {
    }

    void operator()(I& i1, I& i2)
    {
        mFramework = doim::CxxFramework::unique(string(i1, i2));
    }

    doim::CxxFrameworkSPtr mFramework;
};

struct FrameworkSet
{
    FrameworkSet(Framework& framework)
        : mFramework(framework)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mFrameworks.clear(); });
    }

    void operator()(I& i1, I& i2)
    {
        mFrameworks.insert(mFramework.mFramework);
    };

    doim::CxxFrameworkSet mFrameworks;
    Framework& mFramework;
};
}
