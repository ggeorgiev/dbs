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

namespace parser
{
static auto r_depositoryKw = r_str("depository");
static auto r_gitKw = r_str("git");

struct DepositoryRef
{
    DepositoryRef(Object& object)
        : mObject(object)
    {
    }

    void operator()(I& i1, I& i2)
    {
        mDepository = dom::Depository::obtain(mObject.mObject);
    }

    Object& mObject;
    dom::DepositorySPtr mDepository;
};

struct Depository
{
    Depository(Url& url, DepositoryRef& depositoryRef)
        : mUrl(url)
        , mDepositoryRef(depositoryRef)
    {
    }

    auto gitUrl()
    {
        return e_ref([this](I& i1, I& i2) { mDepository->updateGitUrl(mUrl.mUrl); });
    }

    auto name()
    {
        return e_ref([this](I& i1, I& i2) { mDepository = mDepositoryRef.mDepository; });
    }

    Url& mUrl;
    DepositoryRef& mDepositoryRef;

    dom::DepositorySPtr mDepository;
};
}
