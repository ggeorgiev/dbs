//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>
#include <bitset>

#define BITMASK_1(WHAT, X) WHAT(X)
#define BITMASK_2(WHAT, X, ...) WHAT(X) BITMASK_1(WHAT, __VA_ARGS__)
#define BITMASK_3(WHAT, X, ...) WHAT(X) BITMASK_2(WHAT, __VA_ARGS__)
#define BITMASK_4(WHAT, X, ...) WHAT(X) BITMASK_3(WHAT, __VA_ARGS__)
#define BITMASK_5(WHAT, X, ...) WHAT(X) BITMASK_4(WHAT, __VA_ARGS__)

#define BITMASK_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define BITMASK_FOR_EACH(action, ...)                                                       \
    BITMASK_GET_MACRO(__VA_ARGS__, BITMASK_5, BITMASK_4, BITMASK_3, BITMASK_2, BITMASK_1, ) \
    (action, __VA_ARGS__)

#define BITMASK0(X) (1 << (X))
#define BITMASK0_FROM_INDEX(...) (0 + BITMASK_FOR_EACH(BITMASK0, __VA_ARGS__))

#define BITMASK1(X) (1 << ((X)-64))
#define BITMASK1_FROM_INDEX(...) (0 + BITMASK_FOR_EACH(BITMASK1, __VA_ARGS__))

namespace parser
{
template <typename C>
class Token
{
public:
    typedef C Code;

    enum Index
    {
        kKeywordCppProgram = 0,
        kOperatorColon,
        kPath,
        kWhiteSpace,

        kCount
    };

    typedef std::bitset<kCount> Type;

    static Type typeBody(size_t position, Code code)
    {
        Type type;

        switch (code)
        {
            case 0:
            case '\\':
            case ',':
            case ';':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
                break;

            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                type.set(kWhiteSpace);
                break;

            case ':':
                switch (position)
                {
                    case 0:
                        type.set(kOperatorColon);
                        break;
                }
                break;

            case '_':
                type.set(kPath);
                switch (position)
                {
                    case 3:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'a':
                type.set(kPath);
                switch (position)
                {
                    case 9:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'c':
                type.set(kPath);
                switch (position)
                {
                    case 0:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'o':
                type.set(kPath);
                switch (position)
                {
                    case 6:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'g':
                type.set(kPath);
                switch (position)
                {
                    case 7:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;
            case 'm':
                type.set(kPath);
                switch (position)
                {
                    case 10:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'r':
                type.set(kPath);
                switch (position)
                {
                    case 5:
                    case 8:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'p':
                type.set(kPath);
                switch (position)
                {
                    case 1:
                    case 2:
                    case 4:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            default:
                type.set(kPath);
                break;
        }

        return type;
    }
    static bool isidentifier(Code code)
    {
        if (isalpha(code))
            return true;
        if (isdigit(code))
            return true;
        if (code == '_')
            return true;
        return false;
    }

    static Type typeEnd(const Type& current, size_t position, Code code)
    {
        Type type;

        switch (position)
        {
            case 1:
                type.set(kOperatorColon, current[kOperatorColon]);
                break;
            case 11:
                if (!isidentifier(code))
                    type.set(kKeywordCppProgram, current[kKeywordCppProgram]);
                break;
        }

        switch (code)
        {
            case 0:
            case '\\':
            case ',':
            case ';':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
                type.set(kWhiteSpace, current[kWhiteSpace]);
                type.set(kPath, current[kPath]);
                break;

            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                type.set(kPath, current[kPath]);
                break;

            default:
                type.set(kWhiteSpace, current[kWhiteSpace]);
        }

        return type;
    }
};
}
