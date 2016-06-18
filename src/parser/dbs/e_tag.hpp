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
    Tag()
    {
    }

    void operator()(I& i1, I& i2)
    {
        mTag = doim::Tag::unique(string(i1, i2));
    };

    doim::TagSPtr mTag;
};
}
