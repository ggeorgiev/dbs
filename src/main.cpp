//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "db/database.h"
#include "doim/fs/fs_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include "doim/manager.h"
#include "dom/cxx/cxx_program.hpp"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include "parser/parser.hpp"
#include "parser/string_stream.hpp"
#include "tool/cxx_compiler.hpp"

// IWYU pragma: no_include <sstream>

typedef parser::StringStream<char> Stream;
typedef std::shared_ptr<Stream> StreamSPtr;

typedef parser::Parser<Stream> Parser;
typedef std::shared_ptr<Parser> ParserSPtr;

ECode run(const doim::FsFileSPtr& dbsFile, dom::CxxProgramSPtr& program)
{
    std::ifstream fstream(dbsFile->path(nullptr));
    std::string str((std::istreambuf_iterator<char>(fstream)),
                    std::istreambuf_iterator<char>());

    auto stream = std::make_shared<Stream>();
    EHTest(stream->initialize(str));

    auto parser = std::make_shared<Parser>();
    EHTest(parser->initialize(stream, dbsFile));

    EHTest(parser->parse());

    program = parser->cxxProgram();

    EHEnd;
}

int main(int argc, char* argv[])
{
    // TODO: replace this with array_view when available
    std::vector<const char*> arg;
    arg.insert(arg.begin(), argv, argv + argc);

    im::InitializationManager im;

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

    dom::CxxProgramSPtr program;

    code = run(file, program);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    const auto& binary = doim::gManager->obtainFile(cwd, "clang/bin/clang++");
    const auto& compiler = std::make_shared<tool::CxxCompiler>(binary);
    std::string script;
    code = compiler->command(cwd, program, script);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    std::cout << script;
    return 0;
}
