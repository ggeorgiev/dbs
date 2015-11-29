//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "parser/parser.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"

#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include "gtest/err.h"
#include "gtest/err_assert.h"

#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>

#include <stddef.h>
#include <memory>
#include <sstream>
#include <unordered_set>

template <typename T>
class ParserTest : public ::testing::Test
{
public:
    typedef T Parser;
    typedef std::shared_ptr<Parser> ParserSPtr;

    typedef typename Parser::Stream Stream;
    typedef typename Parser::StreamSPtr StreamSPtr;
};

typedef ::testing::Types<parser::Parser<parser::StringStream<char>>> ParserType;

TYPED_TEST_CASE(ParserTest, ParserType);

TYPED_TEST(ParserTest, initialize)
{
    typename TestFixture::StreamSPtr stream;
    dom::FsFileSPtr location;
    auto parser = std::make_shared<typename TestFixture::Parser>();
    ASSERT_EHASSERT(parser->initialize(stream, location));

    stream = std::make_shared<typename TestFixture::Stream>();
    ASSERT_EHASSERT(parser->initialize(stream, location));
    stream.reset();

    location = dom::gFsManager->obtainFile(nullptr, "/foo.dbs");
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
        Test{.files = "", .count = 0},
        Test{.files = "   ", .count = 0},
        Test{.files = "foo", .count = 1},
        Test{.files = "  foo  ", .count = 1},
        Test{.files = "foo  bar foo bar", .count = 2},
    };

    auto directory = dom::gFsManager->obtainDirectory(nullptr, "/");
    auto location = dom::gFsManager->obtainFile(nullptr, "/foo.dbs");

    for (auto test : tests)
    {
        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.files);

        auto parser = std::make_shared<typename TestFixture::Parser>();
        ASSERT_OKAY(parser->initialize(stream, location));

        std::unordered_set<dom::FsFileSPtr> files;
        ASSERT_OKAY(parser->parseFiles(directory, files));

        ASSERT_EQ(test.count, files.size()) << test.files;
    }
}
