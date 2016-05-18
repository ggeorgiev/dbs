//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tool/tool_command.h"

namespace doim
{
ToolCommandSPtr gBuildToolCommand = ToolCommand::global("build", gBuildToolCommand);
ToolCommandSPtr gGenerateToolCommand =
    ToolCommand::global("generate", gGenerateToolCommand);
ToolCommandSPtr gCoverageToolCommand =
    ToolCommand::global("coverage", gCoverageToolCommand);
ToolCommandSPtr gIwyuToolCommand = ToolCommand::global("iwyu", gIwyuToolCommand);
}
