//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/fixed_encoding_traits.hpp"

#include <string>

template <typename C1, typename C2 = C1, typename E = fixed_encoding_traits<C1, C2>>
class StringStream
{
public:
    typedef C1 Char;
    typedef C2 Code;
    typedef E EncodingTraits;

    typedef std::basic_string<Char> string;

    StringStream() : mPosition(0) {}
    void initialize(const string& str)
    {
        mString = str;
        mPosition = mString.c_str();
    }

    bool has() { return mPosition != nullptr; }
    Code next()
    {
        ASSERT(has());

        int ch = EncodingTraits::to_code(mPosition);
        if (ch == 0)
            mPosition = nullptr;
        else
            mPosition += EncodingTraits::sequence_length(mPosition);

        return ch;
    }

private:
    const Char* mPosition;
    string mString;
};
