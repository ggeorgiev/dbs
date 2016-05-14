//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/command.h"
#include "doim/manager.h"
#include "doim/sys/argument.h"
#include "err/err_assert.h"
#include <algorithm>
#include <functional>
#include <str>
#include <vector>

namespace doim
{
SysCommand::SysCommand(const SysExecutableSPtr& executable,
                       const SysArgumentSetSPtr& arguments)
    : Base(executable, arguments)
{
    ASSERT(gManager->isUnique(executable));
    ASSERT(gManager->isUnique(arguments));
}

string SysCommand::toString() const
{
    auto fn = [this]() -> string {
        std::vector<string> strings;
        for (const auto& argument : *arguments())
            strings.push_back(argument->value());

        std::sort(strings.begin(), strings.end());

        string result = executable()->path();
        for (const auto& str : strings)
            result += " " + str;

        return result;
    };

    return mCommandMemoization.get(fn);
}
}
