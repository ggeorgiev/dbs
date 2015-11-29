#include "parser/parser.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"

#include "dom/cpp/cpp_program.hpp"
#include "dom/fs/fs_manager.h"

#include "im/initialization_manager.hpp"

#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <memory>

typedef parser::StringStream<char> Stream;
typedef std::shared_ptr<Stream> StreamSPtr;

typedef parser::Parser<Stream> Parser;
typedef std::shared_ptr<Parser> ParserSPtr;

ECode run(const dom::FsFileSPtr& dbsFile, dom::CppProgramSPtr& program)
{
    std::ifstream t(dbsFile->path(nullptr));
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto stream = std::make_shared<Stream>();
    EHTest(stream->initialize(str));

    auto parser = std::make_shared<Parser>();
    EHTest(parser->initialize(stream, dbsFile));

    EHTest(parser->parse());

    program = parser->cppProgram();

    EHEnd;
}

int main(int argc, const char* argv[])
{
    im::InitializationManager im;

    char current[FILENAME_MAX];
    if (!getcwd(current, sizeof(current)))
        return errno;

    auto cwd = dom::gFsManager->obtainDirectory(nullptr, current);
    auto file = dom::gFsManager->obtainFile(cwd, argv[1]);

    dom::CppProgramSPtr program;

    ECode code = run(file, program);
    if (code != err::kSuccess)
    {
        std::cout << err::gError->message() << "\n";
        std::cout << err::gError->callstack() << "\n";
        return 1;
    }

    std::string script;
    EHTest(program->dumpShell(cwd, script));

    std::cout << script;
    return 0;
}
