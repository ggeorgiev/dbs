//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/generic/attribute.h"

namespace doim
{
Attribute::Attribute(const AttributeNameSPtr& name, const AttributeValueSPtr& value)
    : Element(name, value)
{
    ASSERT(name->isUnique());
    ASSERT(value->isUnique());
}
}
