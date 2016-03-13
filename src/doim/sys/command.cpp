//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/command.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/argument.hpp"
#include "err/err_assert.h"
#include <string>

namespace doim
{
SysCommand::SysCommand(const SysExecutableSPtr& executable,
                       const SysArgumentSetSPtr& arguments)
    : Base(executable, arguments)
{
    ASSERT(gManager->isUnique(executable));
    ASSERT(gManager->isUnique(arguments));
}

const std::string& SysCommand::string() const
{
    if (mString.empty())
    {
        mString = executable()->path();

        for (const auto& argument : *arguments())
            mString += " " + argument->value();
    }

    return mString;
}
}
