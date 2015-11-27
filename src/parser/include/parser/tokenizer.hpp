//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/token.hpp"

#include "err/err_assert.h"

#include <memory>

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

    void initialize(const StreamSPtr& stream)
    {
        ASSERT(stream != nullptr);
        mStream = stream;
    }

    typename Token::Type next()
    {
        if (!mStream->has())
            return Token::Type::kNil;

        auto ch = mStream->next();
        mTokenTypes = Token::types(0, ch);
        mPosition = 1;

        return expand();
    }

    typename Token::Type expand()
    {
        while (mStream->has())
        {
            auto ch = mStream->next();
            auto mask = Token::types(mPosition, ch);

            auto types = mTokenTypes & ~mask;

            mTokenTypes &= mask;
            ++mPosition;

            if (types != Token::Type::kNil)
                return types;
        }

        return mTokenTypes;
    }

    size_t length() { return mPosition - 1; }
private:
    size_t mPosition;
    typename Token::Type mTokenTypes;

    StreamSPtr mStream;
};
