//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/token_type.hpp"

#include <string>

namespace parser
{
template <typename C>
class Token
{
public:
    typedef C Code;
    typedef TokenType Type;

    static Type types(int position, Code code)
    {
        Type type = Type::kNil;

        switch (code)
        {
            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                type |= Type::kWhiteSpace;
                break;

            case '\\':
            case ':':
            case ',':
            case ';':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
                break;

            default:
                type |= Type::kPath;
                break;
        }

        return type;
    }

private:
    Type mTypes;
};
}
