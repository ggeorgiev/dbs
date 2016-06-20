//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class CxxFramework;
typedef shared_ptr<CxxFramework> CxxFrameworkSPtr;
typedef Set<CxxFramework> CxxFrameworkSet;
typedef shared_ptr<CxxFrameworkSet> CxxFrameworkSetSPtr;

class CxxFramework : public Element<CxxFramework, string>
{
public:
    CxxFramework(const string& framework);

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};
}
