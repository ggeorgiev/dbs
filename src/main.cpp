#include "parser/parser.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"

#include "err/err.h"

#include "im/initialization_manager.hpp"

#include <iostream>
#include <fstream>

typedef parser::StringStream<char> Stream;
typedef std::shared_ptr<Stream> StreamSPtr;

typedef parser::Parser<Stream> Parser;
typedef std::shared_ptr<Parser> ParserSPtr;

int main(int argc, const char* argv[])
{
    im::InitializationManager im;

    auto location = dom::gFsManager->obtainFile(nullptr, "/Users/george/github/dbs/src/main.dbs");

    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto stream = std::make_shared<Stream>();
    stream->initialize(str);

    auto parser = std::make_shared<Parser>();
    parser->initialize(stream, location);

    parser->parse();

    auto directory = dom::gFsManager->obtainDirectory(nullptr, "/Users/george/github/dbs");

    auto cppProgram = parser->cppProgram();

    std::string script;
    cppProgram->dumpShell(directory, script);

    std::cout << script;
    return 0;
}
