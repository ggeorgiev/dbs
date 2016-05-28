//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/sys_command.h"
#include "doim/sys/sys_argument.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include <algorithm>
#include <functional>
#include <str>
#include <vector>
#include <stddef.h>

namespace doim
{
SysCommand::SysCommand(const SysExecutableSPtr& executable,
                       const SysArgumentSetSPtr& arguments)
    : Element(executable, arguments)
{
    ASSERT(executable->isUnique());
    ASSERT(arguments->isUnique());
}

string SysCommand::toString() const
{
    auto fn = [this]() -> string {
        std::vector<string> strings;
        for (const auto& argument : *arguments())
            strings.push_back(argument->value());

        std::sort(strings.begin(), strings.end());

        string result = executable()->path();
        size_t line = result.size();
        for (const auto& str : strings)
        {
            line += str.size() + 1;
            if (line > 80)
            {
                line = str.size() + 4;
                result += " \\\n   ";
            }
            result += " " + str;
        }

        return result;
    };

    return mCommandMemoization.get(fn);
}
}
