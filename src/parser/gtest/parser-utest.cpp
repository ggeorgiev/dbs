//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/parser.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err.h"
#include "err/gtest/err.h"
#include "err/gtest/err_assert.h"
#include "gtest/framework.h"
#include <limits>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <stddef.h>

template <typename T>
class ParserTest : public ::testing::Test
{
public:
    typedef T Parser;
    typedef shared_ptr<Parser> ParserSPtr;

    typedef typename Parser::Stream Stream;
    typedef typename Parser::StreamSPtr StreamSPtr;
};

typedef ::testing::Types<parser::Parser<parser::StringStream<char>>> ParserType;

TYPED_TEST_CASE(ParserTest, ParserType);

TYPED_TEST(ParserTest, initialize)
{
    typename TestFixture::StreamSPtr stream;
    doim::FsFileSPtr location;
    auto parser = std::make_shared<typename TestFixture::Parser>();
    ASSERT_EHASSERT(parser->initialize(stream, location));

    stream = std::make_shared<typename TestFixture::Stream>();
    ASSERT_EHASSERT(parser->initialize(stream, location));
    stream.reset();

    location = doim::gManager->obtainFile(nullptr, "/foo.dbs");
    ASSERT_EHASSERT(parser->initialize(stream, location));
}

TYPED_TEST(ParserTest, parseFiles)
{
    struct Test
    {
        std::string files;
        size_t count;
    };

    Test tests[]{
        Test{.files = ";", .count = 0},
        Test{.files = "   ;", .count = 0},
        Test{.files = "foo;", .count = 1},
        Test{.files = "  foo  ;", .count = 1},
        Test{.files = "foo  bar foo bar;", .count = 2},
    };

    auto directory = doim::gManager->obtainDirectory(nullptr, "/");
    auto location = doim::gManager->obtainFile(nullptr, "/foo.dbs");

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "Files: \"" << test.files << "\"");

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.files);

        auto parser = std::make_shared<typename TestFixture::Parser>();
        ASSERT_OKAY(parser->initialize(stream, location));

        doim::FsFileSet files;
        ASSERT_OKAY(
            parser->parseFiles(directory, std::numeric_limits<size_t>::max(), files));

        ASSERT_EQ(test.count, files.size()) << test.files;
    }
}
