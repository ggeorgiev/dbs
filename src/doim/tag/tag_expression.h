//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tag/tag.h"
#include "doim/element.hpp"
#include <boost/hana/for_each.hpp>
#include <memory>
#include <tuple>

namespace doim
{
class TagExpression;
typedef shared_ptr<TagExpression> TagExpressionSPtr;

struct TagExpressionEnums
{
    enum class ETurn
    {
        kOn,
        kOff,
    };
};

class TagExpression : public TagExpressionEnums,
                      public Element<TagExpression,
                                     TagExpressionEnums::ETurn,
                                     TagSetSPtr,
                                     TagExpressionSPtr>
{
public:
    static bool match(const TagSetSPtr& pattern, const TagSetSPtr& tags);

    TagExpression(const ETurn turn,
                  const TagSetSPtr& pattern,
                  const TagExpressionSPtr& next);

    const ETurn turn() const
    {
        return std::get<0>(mArgs);
    }

    const TagSetSPtr& pattern() const
    {
        return std::get<1>(mArgs);
    }

    const TagExpressionSPtr& next() const
    {
        return std::get<2>(mArgs);
    }

    bool match(const TagSetSPtr& tags) const;
};
}
