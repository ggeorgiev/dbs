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
    typedef typename Stream::CharTraits CharTraits;

    typedef Token<Char, CharTraits> Token;
    typedef std::shared_ptr<Token> TokenSPtr;

    void initialize(const StreamSPtr& stream)
    {
        ASSERT(stream != nullptr);
        mStream = stream;
    }

    bool next() { return false; }
private:
    StreamSPtr mStream;
};
