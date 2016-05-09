//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace doim
{
class FsBinary;
typedef std::shared_ptr<FsBinary> FsBinarySPtr;

class FsBinary : public Base<FsBinary, std::string>
{
public:
    FsBinary(const std::string& name);

    const std::string& name() const
    {
        return std::get<0>(mArgs);
    }
};
}
