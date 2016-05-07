//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"
#include "gtest/framework.h"
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
        parser::CxxParser::EIncludeType type;
        std::string includes[10];
    };

    Test tests[]{
        Test{.file = "#include \"foo\"",
             .type = parser::CxxParser::EIncludeType::kProgrammerDefined,
             .includes = {"foo"}},
        Test{.file = "#include \"foo\"\n#include \"bar\"",
             .type = parser::CxxParser::EIncludeType::kProgrammerDefined,
             .includes = {"foo", "bar"}},
        Test{.file = "  #  include   \"foo\"  ",
             .type = parser::CxxParser::EIncludeType::kProgrammerDefined,
             .includes = {"foo"}},
        Test{.file = "  \t#  \tinclude   \t\"foo\"  \t",
             .type = parser::CxxParser::EIncludeType::kProgrammerDefined,
             .includes = {"foo"}},
        Test{.file = "#include <foo>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo"}},
        Test{.file = "#include <foo/bar>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo/bar"}},
    };

    typename TestFixture::CxxParser parser;

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "File: \"" << test.file << "\"");

        auto includes = parser.includes(test.file);

        for (size_t i = 0; i < includes.size(); ++i)
        {
            EXPECT_EQ(test.type, includes[i].mType);
            EXPECT_EQ(test.includes[i], includes[i].mPath);
        }
        EXPECT_EQ("", test.includes[includes.size()]);
    }
}
