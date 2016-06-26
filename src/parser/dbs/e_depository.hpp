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

namespace parser
{
static auto r_depositoryKw = r_str("depository");
static auto r_gitKw = r_str("git");
static auto r_tagKw = r_str("tag");
static auto r_urlKw = r_str("url");

static auto r_dot = r_char('.');

static auto r_gittagIllegal = r_any("~^:*?[\\;");
static auto r_gittagChars = r_any() - r_gittagIllegal - r_space - r_endl;
static auto r_gittagStartChars = r_gittagChars - r_slash;
static auto r_gittagEndChars = r_gittagChars - r_slash - r_dot;
static auto r_gittagValue = r_gittagStartChars & *r_gittagChars;

typedef ObjectRef<dom::Depository> DepositoryRef;

struct Depository
{
    Depository(const doim::FsDirectorySPtr& location)
        : mDepositoryRef(location)
    {
    }

    auto gitUrl()
    {
        return e_ref([this](I& i1, I& i2) { mDepository->updateGitUrl(mUrl.mUrl); });
    }

    auto gitTag()
    {
        return e_ref([this](I& i1, I& i2) { mDepository->updateGitTag(string(i1, i2)); });
    }

    auto name()
    {
        return e_ref([this](I& i1, I& i2) { mDepository = mDepositoryRef.mObjectRef; });
    }

    template <typename WS>
    auto r_depositoryName(const WS& r_ws)
    {
        return mDepositoryRef.rule(r_ws) >> name();
    }

    template <typename WS>
    auto r_giturl(const WS& r_ws)
    {
        return r_ws & r_urlKw & r_ws & r_colon & mUrl.rule(r_ws) >> gitUrl() & r_ws &
               r_semicolon;
    }

    template <typename WS>
    auto r_gittag(const WS& r_ws)
    {
        return r_ws & r_tagKw & r_ws & r_colon & r_ws & r_gittagValue >> gitTag() & r_ws &
               r_semicolon;
    }

    template <typename WS>
    auto r_git(const WS& r_ws)
    {
        return r_ws & r_gitKw & r_ws & r_colon & r_giturl(r_ws) & r_gittag(r_ws) & r_ws &
               r_semicolon;
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_depositoryKw & r_depositoryName(r_ws) & r_ws & r_colon &
               r_git(r_ws) & r_ws & r_semicolon;
    }

    Url mUrl;
    DepositoryRef mDepositoryRef;

    dom::DepositorySPtr mDepository;
};
}
