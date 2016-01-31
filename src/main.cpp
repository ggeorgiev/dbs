//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_compiler.hpp"
#include "parser/parser.hpp"
#include "parser/string_stream.hpp"
#include "dom/cxx/cxx_program.hpp"
#include "dom/manager.h"
#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/generic/object.hpp"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

// IWYU pragma: no_include <sstream>

typedef parser::StringStream<char> Stream;
typedef std::shared_ptr<Stream> StreamSPtr;

typedef parser::Parser<Stream> Parser;
typedef std::shared_ptr<Parser> ParserSPtr;

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

    const auto& binary = doim::gManager->obtainFile(cwd, "clang/bin/clang++");
    const auto& compiler = std::make_shared<tool::CxxCompiler>(binary);

    for (size_t i = 2; i < arg.size(); ++i)
    {
        auto object = doim::gManager->obtainObject(file->directory(),
                                                   doim::Object::Type::kCxxProgram,
                                                   arg[i]);
        auto program = dom::gManager->obtainCxxProgram(object);

        if (program != nullptr)
        {
            std::string script;
            code = compiler->command(cwd, program, script);
            if (code != err::kSuccess)
            {
                std::cout << err::gError->message() << "\n";
                std::cout << err::gError->callstack() << "\n";
                return 1;
            }

            std::cout << script;
        }
    }
    return 0;
}
