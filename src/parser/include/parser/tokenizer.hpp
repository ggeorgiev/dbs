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

    typename Token::Type next()
    {
        if (!mStream->has())
            return Token::Type::kNil;

        auto ch = mStream->take();
        mTokenTypes = Token::types(0, ch);
        mPosition = 1;

        return expand();
    }

    typename Token::Type expand()
    {
        while (mStream->has())
        {
            auto ch = mStream->get();
            auto mask = Token::types(mPosition, ch);

            auto types = mTokenTypes & ~mask;

            mTokenTypes &= mask;

            if (types != Token::Type::kNil)
                return types;

            ++mPosition;
            mStream->move();
        }

        return mTokenTypes;
    }

    size_t length() { return mPosition; }
private:
    size_t mPosition;
    typename Token::Type mTokenTypes;

    StreamSPtr mStream;
};
}
