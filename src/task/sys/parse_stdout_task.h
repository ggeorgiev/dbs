//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/sys/command.h"
#include "err/err.h"
#include "rtti/class_rtti.hpp"
#include <fmt/format.h>
#include <functional>
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class ParseStdoutTask;
typedef shared_ptr<ParseStdoutTask> ParseStdoutTaskSPtr;

class ParseStdoutTask : public Base<ParseStdoutTask,
                                    doim::SysCommandSPtr,
                                    doim::FsDirectorySPtr,
                                    rtti::ClassId>
{
public:
    static std::function<ECode(int, const string&)> logOnError();

    ParseStdoutTask(const doim::SysCommandSPtr& command,
                    const doim::FsDirectorySPtr& targetDirectory,
                    rtti::ClassId parseId,
                    std::function<ECode(int, const string&)> parse,
                    const string& description);

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    doim::FsDirectorySPtr targetDirectory() const
    {
        return std::get<1>(mArgs);
    }

    ECode operator()() override;
    string description() const override;

private:
    std::function<ECode(int, const string&)> mParse;
    string mDescription;
};
}
