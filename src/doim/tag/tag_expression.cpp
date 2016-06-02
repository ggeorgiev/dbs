//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag_expression.h"
#include "doim/set.hpp"
#include "err/err_assert.h"

namespace doim
{
TagExpression::TagExpression(const ETurn turn,
                             const TagSetSPtr& pattern,
                             const TagExpressionSPtr& next)
    : Element(turn, pattern, next)
{
    ASSERT(pattern->isUnique());
    ASSERT(next == nullptr || next->isUnique());
}

bool TagExpression::match(const TagSetSPtr& pattern, const TagSetSPtr& tags)
{
    for (const auto& tag : *pattern)
    {
        if (tags->count(tag) == 0)
            return false;
    }
    return true;
}

bool TagExpression::match(const TagSetSPtr& tags) const
{
    if (match(pattern(), tags))
        return turn() == ETurn::kOn;

    if (next() != nullptr)
        return next()->match(tags);

    return false;
}
}
