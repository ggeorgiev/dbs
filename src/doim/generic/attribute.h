//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/set.hpp"
#include <memory>
#include <tuple>

namespace doim
{
class Attribute;

typedef shared_ptr<Attribute> AttributeSPtr;
typedef Set<Attribute> AttributeSet;
typedef shared_ptr<AttributeSet> AttributeSetSPtr;

class Attribute : public Element<Attribute, AttributeNameSPtr, AttributeValueSPtr>
{
public:
    Attribute(const AttributeNameSPtr& name, const AttributeValueSPtr& value);

    const AttributeNameSPtr& name() const
    {
        return std::get<0>(mArgs);
    }

    const AttributeValueSPtr& value() const
    {
        return std::get<1>(mArgs);
    }
};
}
