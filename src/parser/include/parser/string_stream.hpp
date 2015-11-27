//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/fixed_encoding_traits.hpp"

#include <string>

namespace parser
{

extern const char* kEmptyStringLiteral;

template <typename C1, typename C2 = C1, typename E = fixed_encoding_traits<C1, C2>>
class StringStream
{
public:
    typedef C1 Char;
    typedef C2 Code;
    typedef E EncodingTraits;

    typedef std::basic_string<Char> string;

    StringStream() : mPosition(kEmptyStringLiteral) {}
    void initialize(const string& str)
    {
        mString = str;
        mPosition = mString.c_str();
    }

    bool has() { return *mPosition != 0; }
    Code get()
    {
        ASSERT(has());
        return EncodingTraits::to_code(mPosition);
    }

    Code take()
    {
        ASSERT(has());
        auto ch = EncodingTraits::to_code(mPosition);
        move();
        return ch;
    }

    void move()
    {
        ASSERT(has());
        mPosition += EncodingTraits::sequence_length(mPosition);
    }

private:
    const Char* mPosition;
    string mString;
};
}
