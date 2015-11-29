//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/fixed_encoding_traits.hpp"

#include "err/err.h"

#include "const/constants.h"

#include <string>

namespace parser
{
template <typename C1, typename C2 = C1, typename E = fixed_encoding_traits<C1, C2>>
class StringStream
{
public:
    typedef C1 Char;
    typedef C2 Code;
    typedef E EncodingTraits;

    typedef std::basic_string<Char> String;

    StringStream() : mPosition(mString.begin()) {}
    ECode initialize(const String& str)
    {
        mString = str;
        mPosition = mString.begin();

        EHEnd;
    }

    String range(const typename String::const_iterator& begin,
                 const typename String::const_iterator& end)
    {
        return String(begin, end);
    }

    typename String::const_iterator iterator() const { return mPosition; }
    bool has() { return mPosition != mString.end(); }
    Code get()
    {
        ASSERT(has());
        return EncodingTraits::to_code(&*mPosition);
    }
    Code zget() { return EncodingTraits::to_code(&*mPosition); }
    Code take()
    {
        ASSERT(has());
        auto ch = EncodingTraits::to_code(&*mPosition);
        move();
        return ch;
    }

    void move()
    {
        ASSERT(has());
        mPosition += EncodingTraits::sequence_length(&*mPosition);
    }

private:
    String mString;
    typename String::const_iterator mPosition;
};
}
