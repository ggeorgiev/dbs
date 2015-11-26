#include "parser/tokenizer.hpp"
#include "parser/string_stream.hpp"

#include "gtest/err_assert.h"

#include <gtest/gtest.h>

template <typename T>
class TokenizerTest : public ::testing::Test
{
public:
    typedef T Tokenizer;
    typedef std::shared_ptr<Tokenizer> TokenizerSPtr;

    typedef typename Tokenizer::Stream Stream;
    typedef typename Tokenizer::StreamSPtr StreamSPtr;
};

typedef ::testing::Types<Tokenizer<StringStream<char>>> TokenizerType;

TYPED_TEST_CASE(TokenizerTest, TokenizerType);

TYPED_TEST(TokenizerTest, initiliaze)
{
    typename TestFixture::TokenizerSPtr tokenizer =
        std::make_shared<typename TestFixture::Tokenizer>();

    typename TestFixture::StreamSPtr stream;
    EXPECT_ASSERT(tokenizer->initialize(stream));

    stream = std::make_shared<typename TestFixture::Stream>();
    tokenizer->initialize(stream);
}
