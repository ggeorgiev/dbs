//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_file.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/tag/tag.h"
#include "err/err.h"

namespace parser
{
struct TagSet
{
    TagSet(Tag& tag)
        : mTag(tag)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mTags = doim::TagSet::make(); });
    }

    void operator()(I& i1, I& i2)
    {
        mTags->insert(mTag.mTag);
    };

    doim::TagSetSPtr mTags;
    Tag& mTag;
};
}
