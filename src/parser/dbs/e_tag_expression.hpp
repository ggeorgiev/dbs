//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_tag.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/tag/tag_expression.h"
#include "err/err.h"

namespace parser
{
static auto r_turnChar = r_char('+') | r_char('-');

struct TagExpression
{
    auto turn()
    {
        return e_ref([this](I& i1, I& i2) {
            mTurn = *i1 == '+' ? doim::TagExpression::ETurn::kOn
                               : doim::TagExpression::ETurn::kOff;
        });
    }

    auto section()
    {
        return e_ref([this](I& i1, I& i2) {
            mSection.push_back(std::make_pair(mTurn, doim::TagSet::unique(mTags.mTags)));
        });
    }

    auto combine()
    {
        return e_ref([this](I& i1, I& i2) {
            mTagExpression.reset();
            for (auto it = mSection.rbegin(); it != mSection.rend(); ++it)
            {
                mTagExpression =
                    doim::TagExpression::unique(it->first, it->second, mTagExpression);
            }
        });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return (+(r_ws & r_turnChar >> turn() & mTags.rule(r_ws) >> section())) >>
               combine();
    }

    TagSet mTags;

    std::vector<std::pair<doim::TagExpression::ETurn, doim::TagSetSPtr>> mSection;
    doim::TagExpression::ETurn mTurn;
    doim::TagExpressionSPtr mTagExpression;
};
}
