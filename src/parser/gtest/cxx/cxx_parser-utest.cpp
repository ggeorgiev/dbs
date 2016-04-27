//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h" // IWYU pragma: keep
#include <gtest/gtest-message.h>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <ostream>
#include <stddef.h>

template <typename T>
class CxxParserTest : public ::testing::Test
{
public:
    typedef T CxxParser;
};

typedef ::testing::Types<parser::CxxParser> CxxParserType;

TYPED_TEST_CASE(CxxParserTest, CxxParserType);

TYPED_TEST(CxxParserTest, includes)
{
    struct Test
    {
        std::string file;
        std::string includes[10];
    };

    Test tests[]{
        Test{.file = "#include \"foo\"", .includes = {"\"foo\""}},
        Test{.file = "#include \"foo\"\n#include \"bar\"",
             .includes = {"\"foo\"", "\"bar\""}},
        Test{.file = "  #  include   \"foo\"  ", .includes = {"\"foo\""}},
        Test{.file = "  \t#  \tinclude   \t\"foo\"  \t", .includes = {"\"foo\""}},
        Test{.file = "#include <foo>", .includes = {"<foo>"}},
        Test{.file = "#include <foo/bar>", .includes = {"<foo/bar>"}},
    };

    typename TestFixture::CxxParser parser;

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "File: \"" << test.file << "\"");

        auto includes = parser.includes(test.file);

        for (size_t i = 0; i < includes.size(); ++i)
            EXPECT_EQ(test.includes[i], includes[i]);
        EXPECT_EQ("", test.includes[includes.size()]);
    }
}
