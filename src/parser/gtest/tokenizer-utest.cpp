#include "parser/tokenizer.hpp"
#include "parser/string_stream.hpp"

#include "gtest/err_assert.h"

#include <gtest/gtest.h>

#include <array>

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

typedef ::testing::Types<Tokenizer<StringStream<char>>> TokenizerType;

TYPED_TEST_CASE(TokenizerTest, TokenizerType);

TYPED_TEST(TokenizerTest, initiliaze)
{
    auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();

    typename TestFixture::StreamSPtr stream;
    EXPECT_ASSERT(tokenizer->initialize(stream));

    stream = std::make_shared<typename TestFixture::Stream>();
    tokenizer->initialize(stream);
}

TYPED_TEST(TokenizerTest, empty)
{
    auto stream = std::make_shared<typename TestFixture::Stream>();
    auto tokenizer = std::make_shared<typename TestFixture::Tokenizer>();
    tokenizer->initialize(stream);

    ASSERT_EQ(TokenType::kNil, tokenizer->next());
}

TYPED_TEST(TokenizerTest, path)
{
    std::array<const char*, 11> paths{
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

        ASSERT_EQ(TokenType::kPath, type & TokenType::kPath);

        ASSERT_EQ(14, tokenizer->length()) << path;
    }
}

TYPED_TEST(TokenizerTest, sequence)
{
    struct Sequence
    {
        std::string str;
        std::array<TokenType, 10> types;
    };

    std::array<Sequence, 3> sequences{
        Sequence{.str = "file.cpp",
                 .types =
                     {
                         TokenType::kPath, TokenType::kNil,
                     }},
        Sequence{.str = "directory/file.py",
                 .types =
                     {
                         TokenType::kPath, TokenType::kNil,
                     }},
        Sequence{
            .str = "directory/file.py directory/file.py",
            .types =
                {
                    TokenType::kPath, TokenType::kWhiteSpace, TokenType::kPath, TokenType::kNil,
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

            if (type == TokenType::kNil)
                break;
        }
    }
}
