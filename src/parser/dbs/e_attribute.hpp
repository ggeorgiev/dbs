//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_position.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "err/err.h"

namespace parser
{
static auto r_at = r_char('@');
static auto r_equal = r_char('=');

struct Attribute
{
    auto name()
    {
        return e_ref([this](I& i1, I& i2) {
            mName = doim::AttributeName::unique(string(i1, i2));
        });
    }

    auto value()
    {
        return e_ref([this](I& i1, I& i2) {
            mValue = doim::AttributeValue::unique(string(i1, i2));
        });
    }

    void operator()(I& i1, I& i2)
    {
        mAttribute = doim::Attribute::unique(mName, mValue);
    }

    doim::AttributeNameSPtr mName;
    doim::AttributeValueSPtr mValue;
    doim::AttributeSPtr mAttribute;
};
}
