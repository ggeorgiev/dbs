//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/sys/command.h"
#include "err/err.h"
#include <cppformat/format.h>
#include <functional>
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class ParseStdoutTask;
typedef std::shared_ptr<ParseStdoutTask> ParseStdoutTaskSPtr;

class ParseStdoutTask : public Base<ParseStdoutTask, doim::SysCommandSPtr>
{
public:
    ParseStdoutTask(const doim::SysCommandSPtr& command,
                    std::function<ECode(int, const std::string&)> parse,
                    const std::string& description);

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    ECode operator()() override;
    std::string description() const override;

private:
    std::function<ECode(int, const std::string&)> mParse;
    std::string mDescription;
};
}
