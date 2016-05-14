//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx_clang_format.h"
#include "tool/cxx_compiler.h"
#include "tool/cxx_iwyu.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "parser/parser.hpp"
#include "parser/string_stream.hpp"
#include "dom/manager.h"
#include "doim/fs/fs_binary.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "doim/manager.h"
#include "doim/sys/executable.h"
#include "doim/tag/tag.h"
#include "db/database.h"
#include "err/err.h"
#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

typedef parser::StringStream<char> Stream;
typedef shared_ptr<Stream> StreamSPtr;

typedef parser::Parser<Stream> Parser;
typedef shared_ptr<Parser> ParserSPtr;

ECode run(const doim::FsFileSPtr& dbsFile)
{
    std::ifstream fstream(dbsFile->path(nullptr));
    std::string str((std::istreambuf_iterator<char>(fstream)),
                    std::istreambuf_iterator<char>());

    auto stream = std::make_shared<Stream>();
    EHTest(stream->initialize(str));

    auto parser = std::make_shared<Parser>();
    EHTest(parser->initialize(stream, dbsFile));

    EHTest(parser->parse());
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

    auto cwd = doim::gManager->obtainDirectory(nullptr, current);
    auto file = doim::gManager->obtainFile(cwd, arg[1]);

    ILOG("Load dbs file: {}", file->path(cwd));

    auto db = doim::gManager->obtainDirectory(cwd, "build/db");

    ECode code = db::gDatabase->open(db->path());
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    code = run(file);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    const auto& clangFormatFile =
        doim::gManager->obtainFile(cwd, "clang/bin/clang-format");
    const auto& clangFormat =
        doim::gManager->unique(doim::SysExecutable::make(clangFormatFile));
    const auto& clangFormatTool = std::make_shared<tool::CxxClangFormat>(clangFormat);

    const auto& clangBinary = doim::gManager->unique(doim::FsBinary::make("clang++"));
    const auto& clang = doim::gManager->unique(doim::SysExecutable::make(clangBinary));
    const auto& compiler = std::make_shared<tool::CxxCompiler>(clang);

    const auto& iwyuFile =
        doim::gManager->obtainFile(cwd, "clang/bin/include-what-you-use");
    const auto& iwyu = doim::gManager->unique(doim::SysExecutable::make(iwyuFile));
    const auto& iwyuTool = std::make_shared<tool::CxxIwyu>(iwyu);

    const auto& engine =
        std::make_shared<engine::CxxEngine>(clangFormatTool, compiler, iwyuTool);

    const auto& verb = doim::gManager->find(doim::Tag::make(arg[2]));

    if (verb == nullptr)
    {
        ILOG("Action \"{}\" is not recognized", arg[2]);
        return 1;
    }

    ILOG("Evaluating targets for {}", verb->name());

    std::vector<tpool::TaskSPtr> tasks;
    for (size_t i = 3; i < arg.size(); ++i)
    {
        auto object = doim::gManager->obtainObject(file->directory(),
                                                   doim::Object::EType::kCxxProgram,
                                                   arg[i]);
        auto program = dom::gManager->obtainCxxProgram(object);
        if (program == nullptr)
            continue;

        if (verb == doim::gBuildTag)
        {
            tasks.push_back(
                engine->build(engine::CxxEngine::EBuildFor::kDebug, cwd, program));
        }
        else if (verb == doim::gCoverageTag)
        {
            tasks.push_back(
                engine->build(engine::CxxEngine::EBuildFor::kProfile, cwd, program));
        }
        else if (verb == doim::gIwyuTag)
            tasks.push_back(engine->iwyu(cwd, program));
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
        return 1;
    }

    return 0;
}
