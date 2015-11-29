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

    typedef typename Token::Type Type;
    typedef typename Token::Keyword Keyword;
    typedef typename Token::Operator Operator;

    ECode initialize(const StreamSPtr& stream)
    {
        EHAssert(stream != nullptr);
        mStream = stream;
        EHEnd;
    }

    String token() { return String(mTokeBegin, mStream->iterator()); }
    size_t length() { return mPosition; }
    bool expectOperator(Operator operator_)
    {
        if (!mStream->has())
            return false;

        mTokeBegin = mStream->iterator();

        mTokenTypes = Type::kNil;
        mKeywords = Keyword::kNil;
        mOperators = operator_;
        mPosition = 0;

        return expand() == Type::kOperator;
    }

    Type next()
    {
        if (!mStream->has())
            return Type::kNil;

        mTokeBegin = mStream->iterator();

        auto ch = mStream->take();

        mTokenTypes = Token::types(ch);
        mKeywords = Token::keywordBody(0, ch);
        mOperators = Token::operatorBody(0, ch);
        mPosition = 1;

        return expand();
    }

    Type expand()
    {
        for (;;)
        {
            auto ch = mStream->zget();
            auto types =
                ((mKeywords != Keyword::kNil) && (mKeywords && Token::keywordEnd(mPosition, ch))
                     ? Type::kKeyword
                     : Type::kNil) |
                ((mOperators != Operator::kNil) && (mOperators && Token::operatorEnd(mPosition, ch))
                     ? Type::kOperator
                     : Type::kNil);

            if (!mStream->has())
                return mTokenTypes | types;

            auto mask = mTokenTypes != Type::kNil ? Token::types(ch) : Type::kNil;

            types |= mTokenTypes & ~mask;

            mTokenTypes &= mask;
            if (mKeywords != Keyword::kNil)
                mKeywords &= Token::keywordBody(mPosition, ch);
            if (mOperators != Operator::kNil)
                mOperators &= Token::operatorBody(mPosition, ch);

            if (types != Type::kNil)
                return types;

            if (mTokenTypes == Type::kNil && mKeywords == Keyword::kNil &&
                mOperators == Operator::kNil && )
            {
                return Type::kNil;
            }

            ++mPosition;
            mStream->move();
        }

        return Type::kNil;
    }

private:
    typename String::const_iterator mTokeBegin;

    size_t mPosition;
    Type mTokenTypes;
    Keyword mKeywords;
    Operator mOperators;

    StreamSPtr mStream;
};
}
