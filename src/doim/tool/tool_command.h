//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include <boost/hana/for_each.hpp>
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class ToolCommand;
typedef shared_ptr<ToolCommand> ToolCommandSPtr;

class ToolCommand : public Element<ToolCommand, string>
{
public:
    ToolCommand(const string& name)
        : Element(name)
    {
    }

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};

extern ToolCommandSPtr gBuildToolCommand;
extern ToolCommandSPtr gGenerateToolCommand;
extern ToolCommandSPtr gCoverageToolCommand;
extern ToolCommandSPtr gIwyuToolCommand;
}
