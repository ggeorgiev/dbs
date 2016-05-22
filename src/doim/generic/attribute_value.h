//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class AttributeValue;

typedef shared_ptr<AttributeValue> AttributeValueSPtr;
typedef Set<AttributeValue> AttributeValueSet;
typedef shared_ptr<AttributeValueSet> AttributeValueSetSPtr;

class AttributeValue : public Element<AttributeValue, string>
{
public:
    AttributeValue(const string& value)
        : Element(value)
    {
    }

    const string& value() const
    {
        return std::get<0>(mArgs);
    }
};
}
