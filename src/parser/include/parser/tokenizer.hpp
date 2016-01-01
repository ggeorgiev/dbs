//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/token.hpp"
#include "err/err.h"
#include "err/err_assert.h"
#include <iostream>
#include <memory>

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

    typedef typename Token::Type Type;

    ECode initialize(const StreamSPtr& stream)
    {
        EHAssert(stream != nullptr);
        mStream = stream;
        EHEnd;
    }

    String token()
    {
        return String(mTokeBegin, mStream->iterator());
    }
    size_t length()
    {
        return mPosition;
    }
    Type next()
    {
        if (!mStream->has())
            return Type();

        mTokeBegin = mStream->iterator();

        const auto& ch = mStream->take();

        mTokenTypes = Token::typeBody(Type(), 0, ch);

// #define TOKENIZER_DUMP

#if defined(TOKENIZER_DUMP)
        std::cout << "Position: 0, code: " << ch << ", Current: " << mTokenTypes << "\n";
#endif
        mPosition = 1;

        return expand();
    }

    Type expand()
    {
        for (;;)
        {
            auto ch = mStream->zget();
            auto typeEnd = Token::typeEnd(mTokenTypes, mPosition, ch);
#if defined(TOKENIZER_DUMP)
            std::cout << "Position: " << mPosition << ", code: " << ch
                      << ", Current: " << mTokenTypes << ", End: " << typeEnd << "\n";
#endif

            if (typeEnd.any() || !mStream->has())
                return typeEnd;

            auto typeBody = Token::typeBody(mTokenTypes, mPosition, ch);
#if defined(TOKENIZER_DUMP)
            std::cout << "Body: " << typeBody << "\n";
#endif
            mTokenTypes &= typeBody;

            if (mTokenTypes.none())
                break;

            ++mPosition;
            mStream->move();
        }

        return mTokenTypes;
    }

private:
    typename String::const_iterator mTokeBegin;

    size_t mPosition;
    Type mTokenTypes;
    StreamSPtr mStream;
};
}
