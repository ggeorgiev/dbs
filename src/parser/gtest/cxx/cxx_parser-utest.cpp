//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"
#include "gtest/framework.h"
#include <str>
#include <vector>
#include <stddef.h>

TEST(CxxParserTest, includes)
{
    struct Test
    {
        string file;
        parser::CxxParser::EIncludeType type;
        string includes[10];
    };

    Test tests[]{
        Test{.file = "#include \"foo\"",
             .type = parser::CxxParser::EIncludeType::kProgrammer,
             .includes = {"foo"}},
        Test{.file = "#include \"foo\"\n#include \"bar\"",
             .type = parser::CxxParser::EIncludeType::kProgrammer,
             .includes = {"foo", "bar"}},
        Test{.file = "#include \"foo\"\nsome code\n#include \"bar\"",
             .type = parser::CxxParser::EIncludeType::kProgrammer,
             .includes = {"foo", "bar"}},
        Test{.file = "  #  include   \"foo\"  ",
             .type = parser::CxxParser::EIncludeType::kProgrammer,
             .includes = {"foo"}},
        Test{.file = "  \t#  \tinclude   \t\"foo\"  \t",
             .type = parser::CxxParser::EIncludeType::kProgrammer,
             .includes = {"foo"}},
        Test{.file = "#include <foo>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo"}},
        Test{.file = "#include <foo/bar>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo/bar"}},
        Test{.file = "/*#include <foo>*/",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {}},
        Test{.file = "#include <foo>/*#include <foo>*/",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo"}},
        Test{.file = "// #include <foo>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {}},
        Test{.file = "foo // #include <foo>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {}},
        Test{.file = "something #include <foo>",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {}},
        Test{.file = "#include <foo/bar> // IWYU pragma: keep\n",
             .type = parser::CxxParser::EIncludeType::kStandard,
             .includes = {"foo/bar"}},
    };

    parser::CxxParser parser;

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
