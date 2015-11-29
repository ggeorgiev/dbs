//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/token.hpp"

#include "err/err.h"
#include "err/err_assert.h"

#include <memory>
#include <iostream>

namespace parser
{
template <typename S>
class Tokenizer
{
public:
    typedef S Stream;
    typedef std::shared_ptr<Stream> StreamSPtr;

    typedef typename Stream::String String;

    typedef typename Stream::Char Char;
    typedef typename Stream::Code Code;

    typedef Token<Code> Token;
    typedef std::shared_ptr<Token> TokenSPtr;

    ECode initialize(const StreamSPtr& stream)
    {
        EHAssert(stream != nullptr);
        mStream = stream;
        EHEnd;
    }

    String token() { return String(mTokeBegin, mStream->iterator()); }
    typename Token::Type next()
    {
        if (!mStream->has())
            return Token::Type::kNil;

        mTokeBegin = mStream->iterator();

        auto ch = mStream->take();

        mKeywords = Token::keywordBody(0, ch);
        mTokenTypes = Token::types(ch);
        mPosition = 1;

        return expand();
    }

    typename Token::Type expand()
    {
        for (;;)
        {
            auto ch = mStream->zget();
            auto types =
                ((mKeywords != Keyword::kNil) && (mKeywords && Token::keywordEnd(mPosition, ch)))
                    ? Token::Type::kKeyword
                    : Token::Type::kNil;

            if (!mStream->has())
                return mTokenTypes | types;

            auto mask = Token::types(ch);

            types |= mTokenTypes & ~mask;

            mTokenTypes &= mask;
            if (mKeywords != Keyword::kNil)
                mKeywords |= Token::keywordBody(mPosition, ch);

            if (types != Token::Type::kNil)
                return types;

            ++mPosition;
            mStream->move();
        }

        return Token::Type::kNil;
    }

    size_t length() { return mPosition; }
private:
    typename String::const_iterator mTokeBegin;

    size_t mPosition;
    typename Token::Type mTokenTypes;
    typename Token::Keyword mKeywords;

    StreamSPtr mStream;
};
}
