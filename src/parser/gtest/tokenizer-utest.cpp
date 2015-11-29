#include "parser/string_stream.hpp"
#include "parser/tokenizer.hpp" // IWYU pragma: keep

#include "err/err.h"

#include "gtest/err_assert.h"

#include <gtest/gtest-message.h>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>

template <typename T>
class TokenizerTest : public ::testing::Test
{
public:
    typedef T Tokenizer;
    typedef std::shared_ptr<Tokenizer> TokenizerSPtr;

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

    for (auto test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: \"" << test.str << "\"");

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (auto token : test.tokens)
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

    for (auto path : paths)
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
        Test{.str = "file.cpp",
             .types =
                 {
                     BITMASK0_FROM_INDEX(Token::kPath), 0,
                 }},
        Test{.str = "directory/file.py",
             .types =
                 {
                     BITMASK0_FROM_INDEX(Token::kPath), 0,
                 }},
        Test{.str = "directory/file.py directory/file.py",
             .types =
                 {
                     BITMASK0_FROM_INDEX(Token::kPath),
                     BITMASK0_FROM_INDEX(Token::kWhiteSpace),
                     BITMASK0_FROM_INDEX(Token::kPath),
                     0,
                 }},
    };

    for (auto test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: " << test.str);

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (auto type : test.types)
        {
            SCOPED_TRACE(::testing::Message() << "Type: " << type);

            ASSERT_TRUE((tokenizer->next() & type) == type);

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
        Test{.str = "cpp_program", .expected = Token::kKeywordCppProgram},
        Test{.str = "cpp_program ", .expected = Token::kKeywordCppProgram},
        Test{.str = ":", .expected = Token::kOperatorColon},
        Test{.str = ": ", .expected = Token::kOperatorColon},
    };

    for (auto test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: " << test.str);

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        ASSERT_TRUE(tokenizer->next().test(test.expected));
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
        Test{.str = "cpp_pro", .unexpected = Token::kKeywordCppProgram},
        Test{.str = "cpp_programfoo", .unexpected = Token::kKeywordCppProgram},
        Test{.str = "cpp_program_foo", .unexpected = Token::kKeywordCppProgram},
    };

    for (auto test : tests)
    {
        SCOPED_TRACE(::testing::Message() << "String: " << test.str);

        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        ASSERT_FALSE(tokenizer->next().test(test.unexpected));
    }
}
