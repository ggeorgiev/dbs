//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/token_type.hpp"
#include "parser/keyword.hpp"
#include "parser/operator.hpp"

#include <string>

namespace parser
{
template <typename C>
class Token
{
public:
    typedef C Code;
    typedef TokenType Type;
    typedef Keyword Keyword;
    typedef Operator Operator;

    static Type types(Code code)
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

    static Keyword keywordBody(int position, Code code)
    {
        Keyword keyword = Keyword::kNil;

        switch (position)
        {
            case 0:
                switch (code)
                {
                    case 'c':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 1:
                switch (code)
                {
                    case 'p':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 2:
                switch (code)
                {
                    case 'p':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 3:
                switch (code)
                {
                    case '_':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 4:
                switch (code)
                {
                    case 'p':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 5:
                switch (code)
                {
                    case 'r':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 6:
                switch (code)
                {
                    case 'o':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 7:
                switch (code)
                {
                    case 'g':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 8:
                switch (code)
                {
                    case 'r':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 9:
                switch (code)
                {
                    case 'a':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
            case 10:
                switch (code)
                {
                    case 'm':
                        keyword |= Keyword::kCppProgram;
                        break;
                }
                break;
        }
        return keyword;
    }

    static bool isidentifier(Code code)
    {
        if (isalpha(code))
            return true;
        if (isdigit(code))
            return true;
        return false;
    }

    static Keyword keywordEnd(int position, Code code)
    {
        Keyword keyword = Keyword::kNil;

        switch (position)
        {
            case 11:
                if (!isidentifier(code))
                    keyword |= Keyword::kCppProgram;
                break;
        }
        return keyword;
    }

    static Operator operatorBody(int position, Code code)
    {
        Operator operator_ = Operator::kNil;

        switch (position)
        {
            case 0:
                switch (code)
                {
                    case ':':
                        operator_ |= Operator::kColon;
                        break;
                }
                break;
        }
        return operator_;
    }

    static Operator operatorEnd(int position, Code code)
    {
        Operator operator_ = Operator::kNil;

        switch (position)
        {
            case 1:
                operator_ |= Operator::kColon;
                break;
        }
        return operator_;
    }

private:
    Type mTypes;
};
}
