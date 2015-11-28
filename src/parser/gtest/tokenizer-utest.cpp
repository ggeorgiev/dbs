#include "parser/tokenizer.hpp" // IWYU pragma: keep
#include "parser/string_stream.hpp"
#include "parser/token_type.hpp"

#include "err/err.h"

#include "gtest/err_assert.h"

#include <gtest/gtest-message.h>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>

#include <memory>
#include <sstream>

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

    ASSERT_EQ(parser::TokenType::kNil, tokenizer->next());
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
        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(test.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (auto token : test.tokens)
        {
            if (token.empty())
                break;

            ASSERT_NE(parser::TokenType::kNil, tokenizer->next()) << test.str;
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

        auto type = tokenizer->next();

        ASSERT_EQ(parser::TokenType::kPath, type & parser::TokenType::kPath);

        ASSERT_EQ(14, tokenizer->length()) << path;
    }
}

TYPED_TEST(TokenizerTest, sequence)
{
    struct Sequence
    {
        std::string str;
        parser::TokenType types[10];
    };

    Sequence sequences[]{
        Sequence{.str = "file.cpp",
                 .types =
                     {
                         parser::TokenType::kPath, parser::TokenType::kNil,
                     }},
        Sequence{.str = "directory/file.py",
                 .types =
                     {
                         parser::TokenType::kPath, parser::TokenType::kNil,
                     }},
        Sequence{.str = "directory/file.py directory/file.py",
                 .types =
                     {
                         parser::TokenType::kPath,
                         parser::TokenType::kWhiteSpace,
                         parser::TokenType::kPath,
                         parser::TokenType::kNil,
                     }},
    };

    for (auto sequence : sequences)
    {
        auto stream = std::make_shared<typename TestFixture::Stream>();
        stream->initialize(sequence.str);

        auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
        tokenizer->initialize(stream);

        for (auto type : sequence.types)
        {
            ASSERT_EQ(type, tokenizer->next()) << "in \"" << sequence.str << "\"";

            if (type == parser::TokenType::kNil)
                break;
        }
    }
}
