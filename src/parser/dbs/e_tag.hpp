//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/tag/tag.h"
#include "err/err.h"

namespace parser
{
struct Tag
{
    auto set()
    {
        return e_ref([this](I& i1, I& i2) { mTag = doim::Tag::unique(string(i1, i2)); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_ident() >> set();
    }

    doim::TagSPtr mTag;
};

struct TagSet
{
    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mTags = doim::TagSet::make(); });
    }

    auto insert()
    {
        return e_ref([this](I& i1, I& i2) { mTags->insert(mTag.mTag); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_empty() >> reset() & +(mTag.rule(r_ws) >> insert());
    }

    doim::TagSetSPtr mTags;

    Tag mTag;
};
}
