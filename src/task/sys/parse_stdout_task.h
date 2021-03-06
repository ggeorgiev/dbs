//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "task/element_manager.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/sys/sys_command.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include "rtti/class_rtti.hpp"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
#include <functional>
#include <shared_ptr>
#include <str>
#include <tuple>

namespace task
{
class ParseStdoutTask;
typedef shared_ptr<ParseStdoutTask> ParseStdoutTaskSPtr;

class ParseStdoutTask : public Element<ParseStdoutTask,
                                       doim::SysCommandSPtr,
                                       doim::FsDirectorySPtr,
                                       rtti::ClassId>
{
public:
    static std::function<ECode(int, const string&)> logOnError();

    template <typename... Args>
    static shared_ptr<ParseStdoutTask> valid(const Args&... args)
    {
        return gElementManager->valid(enable_make_shared<ParseStdoutTask>::make(args...));
    }

    ParseStdoutTask(const doim::SysCommandSPtr& command,
                    const doim::FsDirectorySPtr& targetDirectory,
                    rtti::ClassId parseId,
                    std::function<ECode(int, const string&)> parse,
                    const string& description);

    bool check() const;

    doim::SysCommandSPtr command() const
    {
        return std::get<0>(mArgs);
    }

    doim::FsDirectorySPtr targetDirectory() const
    {
        return std::get<1>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;

private:
    std::function<ECode(int, const string&)> mParse;
    string mDescription;
};
}
