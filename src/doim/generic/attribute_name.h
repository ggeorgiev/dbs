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
class AttributeName;

typedef shared_ptr<AttributeName> AttributeNameSPtr;
typedef Set<AttributeName> AttributeNameSet;
typedef shared_ptr<AttributeNameSet> AttributeNameSetSPtr;

class AttributeName : public Element<AttributeName, string>
{
public:
    AttributeName(const string& name)
        : Element(name)
    {
    }

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};
}
