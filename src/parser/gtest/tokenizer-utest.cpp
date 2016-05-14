//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "parser/tokenizer.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"
#include "err/err.h"
#include "err/gtest/err_assert.h"
#include "gtest/framework.h"
#include <bitset>
#include <shared_ptr>
#include <sstream>

template <typename T>
class TokenizerTest : public ::testing::Test
{
public:
    typedef T Tokenizer;
    typedef shared_ptr<Tokenizer> TokenizerSPtr;

    typedef typename Tokenizer::Stream Stream;
    typedef typename Tokenizer::StreamSPtr StreamSPtr;

    typedef typename Tokenizer::Token Token;
    typedef typename Tokenizer::TokenSPtr TokenSPtr;
};

typedef ::testing::Types<parser::Tokenizer<parser::StringStream<char>>> TokenizerType;

TYPED_TEST_CASE(TokenizerTest, TokenizerType);

TYPED_TEST(TokenizerTest, initialize)
{
    auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();

    typename TestFixture::StreamSPtr stream;
    ASSERT_EHASSERT(tokenizer->initialize(stream));

    stream = std::make_shared<typename TestFixture::Stream>();
    tokenizer->initialize(stream);
}

TYPED_TEST(TokenizerTest, empty)
{
    auto stream = std::make_shared<typename TestFixture::Stream>();
    auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
    tokenizer->initialize(stream);

    ASSERT_TRUE(tokenizer->next().none());
}

TYPED_TEST(TokenizerTest, token)
{
    struct Test
    {
        std::string str;
        std::string tokens[10];
    };

    Test tests[]{
        Test{.str = "", .tokens = {""}},
        Test{.str = "  ", .tokens = {"  ", ""}},
        Test{.str = "  foo   ", .tokens = {"  ", "foo", "   ", ""}},
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: \"" << test.str << "\"");

        const auto& stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        const auto& tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (const auto& token : test.tokens)
        {
            SCOPED_TRACE(::testing::Message() << "Token: \"" << token << "\"");

            if (token.empty())
                break;

            ASSERT_TRUE(tokenizer->next().any()) << test.str;
            ASSERT_EQ(token, tokenizer->token()) << test.str;
        }
    }
}

TYPED_TEST(TokenizerTest, path)
{
    const char* paths[]{
        "this_is_a_path",
        "this_is_a_path ",
        "this_is_a_path\n",
        "this/is/a/path",
        "this+is-a~path",
        "this+is-a~path\\",
        "01234567890123 ",
        "this.is.a.path ",
        "this.is.a.path ",
        "this_is_a_path,",
        "this_is_a_path;",
    };

    for (const auto& path : paths)
    {
        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(path);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        ASSERT_TRUE(tokenizer->next().test(TestFixture::Token::kPath));
        ASSERT_EQ(14, tokenizer->length()) << path;
    }
}

TYPED_TEST(TokenizerTest, sequence)
{
    typedef typename TestFixture::Tokenizer::Token Token;
    struct Test
    {
        std::string str;
        typename Token::Type types[10];
    };

    Test tests[]{
        Test{.str = "file.cxx",
             .types =
                 {
                     Token::kPathMask, 0,
                 }},
        Test{.str = "directory/file.py",
             .types =
                 {
                     Token::kPathMask, 0,
                 }},
        Test{.str = "directory/file.py directory/file.py",
             .types =
                 {
                     Token::kPathMask, Token::kWhiteSpaceMask, Token::kPathMask, 0,
                 }},

        Test{.str = "file1.py#file2.py file3.py\nfile4.py",
             .types =
                 {
                     Token::kPathMask,
                     Token::kCommentMask,
                     Token::kWhiteSpaceMask,
                     Token::kPathMask,
                     0,
                 }},
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: " << test.str);

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (const auto& type : test.types)
        {
            SCOPED_TRACE(::testing::Message() << "Type: " << type);

            auto result = tokenizer->next();
            ASSERT_EQ(result & type, type) << "Result: " << result << ", Token: \""
                                           << tokenizer->token() << "\"";

            if (type.none())
                break;
        }
    }
}

TYPED_TEST(TokenizerTest, type)
{
    typedef typename TestFixture::Tokenizer::Token Token;

    struct Test
    {
        const char* str;
        typename Token::Index expected;
    };

    Test tests[]{
        Test{.str = "#foo", .expected = Token::kComment},
        Test{.str = "foo", .expected = Token::kIdentifier},
        Test{.str = "annex", .expected = Token::kKeywordAnnex},
        Test{.str = "cxx_binary", .expected = Token::kKeywordCxxBinary},
        Test{.str = "cxx_file", .expected = Token::kKeywordCxxFile},
        Test{.str = "cxx_header", .expected = Token::kKeywordCxxHeader},
        Test{.str = "cxx_library", .expected = Token::kKeywordCxxLibrary},
        Test{.str = "cxx_program", .expected = Token::kKeywordCxxProgram},
        Test{.str = "=", .expected = Token::kOperatorAssignment},
        Test{.str = "@", .expected = Token::kOperatorAt},
        Test{.str = ":", .expected = Token::kOperatorColon},
        Test{.str = ";", .expected = Token::kOperatorSemicolon},
        Test{.str = "foo/bar", .expected = Token::kPath},
        Test{.str = "  ", .expected = Token::kWhiteSpace},

        // special cases:
        Test{.str = "bar123_  ", .expected = Token::kIdentifier},
        Test{.str = "cxx_program ", .expected = Token::kKeywordCxxProgram},
        Test{.str = "cxx_program:", .expected = Token::kKeywordCxxProgram},
        Test{.str = ": ", .expected = Token::kOperatorColon},
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: \"" << test.str << "\"");

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        auto type = tokenizer->next();
        ASSERT_TRUE(type.test(test.expected)) << "Type: " << type;
    }
}

TYPED_TEST(TokenizerTest, negatove_type)
{
    typedef typename TestFixture::Tokenizer::Token Token;

    struct Test
    {
        const char* str;
        typename Token::Index unexpected;
    };

    Test tests[]{
        Test{.str = "123_", .unexpected = Token::kIdentifier},
        Test{.str = "cxx_pro", .unexpected = Token::kKeywordCxxProgram},
        Test{.str = "cxx_programfoo", .unexpected = Token::kKeywordCxxProgram},
        Test{.str = "cxx_program_foo", .unexpected = Token::kKeywordCxxProgram},
        Test{.str = "cxx_fi", .unexpected = Token::kKeywordCxxFile},
        Test{.str = "cxx_filefoo", .unexpected = Token::kKeywordCxxFile},
    };

    for (const auto& test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: " << test.str);

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        ASSERT_FALSE(tokenizer->next().test(test.unexpected));
    }
}
