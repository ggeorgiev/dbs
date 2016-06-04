//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx/cxx_clang_format.h"
#include "tool/cxx/cxx_compiler.h"
#include "tool/cxx/cxx_iwyu.h"
#include "tool/protobuf/protobuf_compiler.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "parser/dbs/dbs_parser.h"
#include "dom/cxx/cxx_program.h"
#include "option/verbose.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "doim/sys/sys_executable.h"
#include "doim/tool/tool_command.h"
#include "db/database.h"
#include "err/err.h"
#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

ECode run(const doim::FsFileSPtr& dbsFile)
{
    parser::DbsParser parser;
    EHTest(parser.parse(dbsFile));
    EHEnd;
}

int main(int argc, char* argv[])
{
    // TODO: replace this with array_view when available
    std::vector<const char*> arg;
    arg.insert(arg.begin(), argv, argv + argc);

    im::InitializationManager im;

    dbslog::gLogger->set_level(spdlog::level::info);

    char current[FILENAME_MAX];
    if (getcwd(current, sizeof(current)) == nullptr)
        return errno;

    auto cwd = doim::FsDirectory::obtain(nullptr, current);
    auto file = doim::FsFile::obtain(cwd, arg[1]);

    ILOG("Load dbs file: {}", file->path(cwd));

    auto config = doim::FsFile::obtain(file->directory(), "config.dbs");

    ECode code = opt::gVerbose->config(config);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        EHReset;
        return 1;
    }

    auto db = doim::FsDirectory::obtain(cwd, "build/db");

    code = db::gDatabase->open(db->path());
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        EHReset;
        return 1;
    }

    code = run(file);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        EHReset;
        return 1;
    }

    const auto& clangFormatFile = doim::FsFile::obtain(cwd, "clang/bin/clang-format");
    const auto& clangFormat = doim::SysExecutable::unique(clangFormatFile);
    const auto& clangFormatTool = std::make_shared<tool::CxxClangFormat>(clangFormat);

    const auto& clangBinary = doim::FsRelativeFile::unique(nullptr, "clang++");
    const auto& clang = doim::SysExecutable::unique(clangBinary);
    const auto& compiler = std::make_shared<tool::CxxCompiler>(clang);

    const auto& iwyuFile = doim::FsFile::obtain(cwd, "clang/bin/include-what-you-use");
    const auto& iwyu = doim::SysExecutable::unique(iwyuFile);
    const auto& iwyuTool = std::make_shared<tool::CxxIwyu>(iwyu);

    const auto& protobufFile = doim::FsFile::obtain(cwd, "protobuf/bin/protoc");
    const auto& protobuf = doim::SysExecutable::unique(protobufFile);
    const auto& protobufTool = std::make_shared<tool::ProtobufCompiler>(protobuf);

    const auto& engine = std::make_shared<engine::CxxEngine>(clangFormatTool,
                                                             compiler,
                                                             iwyuTool,
                                                             protobufTool);

    const auto& verb = doim::ToolCommand::make(arg[2])->find();

    if (verb == nullptr)
    {
        ILOG("Action \"{}\" is not recognized", arg[2]);
        return 1;
    }

    ILOG("Evaluating targets for {}", verb->name());

    std::vector<tpool::TaskSPtr> tasks;
    for (size_t i = 3; i < arg.size(); ++i)
    {
        auto object = doim::Object::obtain(doim::Object::EType::kCxxProgram,
                                           file->directory(),
                                           arg[i]);
        auto program = dom::CxxProgram::find(object);
        if (program == nullptr)
        {
            ELOG("Failed to find {}", object->name());
            continue;
        }

        if (verb == doim::gBuildToolCommand)
        {
            tasks.push_back(
                engine->build(engine::CxxEngine::EBuildFor::kDebug, cwd, program));
        }
        else if (verb == doim::gCoverageToolCommand)
        {
            tasks.push_back(
                engine->build(engine::CxxEngine::EBuildFor::kProfile, cwd, program));
        }
        else if (verb == doim::gIwyuToolCommand)
        {
            tasks.push_back(engine->iwyu(cwd, program));
        }
        else if (verb == doim::gGenerateToolCommand)
        {
            std::cout << engine->buildScript(engine::CxxEngine::EBuildFor::kRelease,
                                             cwd,
                                             program);
        }
        else
            ASSERT(false);
    }

    auto group = tpool::TaskGroup::make(task::gTPool, 0, tasks);
    task::gTPool->ensureScheduled(group);

    code = group->join();

    task::gTPool->join();

    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        EHReset;
        return 1;
    }

    return 0;
}
