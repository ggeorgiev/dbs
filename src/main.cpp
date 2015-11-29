#include "parser/keyword.hpp"
#include "parser/parser.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"
#include "parser/token_type.hpp"

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

int main(int argc, const char* argv[])
{
    im::InitializationManager im;

    char current[FILENAME_MAX];
    if (!getcwd(current, sizeof(current)))
        return errno;

    auto cwd = dom::gFsManager->obtainDirectory(nullptr, current);
    auto location = dom::gFsManager->obtainFile(cwd, argv[1]);

    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto stream = std::make_shared<Stream>();
    stream->initialize(str);

    auto parser = std::make_shared<Parser>();
    parser->initialize(stream, location);

    parser->parse();

    auto cppProgram = parser->cppProgram();

    std::string script;
    cppProgram->dumpShell(cwd, script);

    std::cout << script;
    return 0;
}
