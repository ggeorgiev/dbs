//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class FsBinary;
typedef shared_ptr<FsBinary> FsBinarySPtr;

class FsBinary : public Element<FsBinary, string>
{
public:
    FsBinary(const string& name);

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};
}
