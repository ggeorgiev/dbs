//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <string>
#include <bitset>

#define BITMASK0(X) static const uint64_t X##Mask = 1 << X

namespace parser
{
template <typename C>
class Token
{
public:
    typedef C Code;

    enum Index
    {
        kIdentifier = 0,
        kKeywordCppProgram,
        kKeywordCppFile,
        kOperatorColon,
        kOperatorSemicolon,
        kPath,
        kWhiteSpace,

        kCount
    };

    BITMASK0(kIdentifier);
    BITMASK0(kKeywordCppProgram);
    BITMASK0(kKeywordCppFile);
    BITMASK0(kOperatorColon);
    BITMASK0(kOperatorSemicolon);
    BITMASK0(kPath);
    BITMASK0(kWhiteSpace);

    typedef std::bitset<kCount> Type;

    static Type typeBody(size_t position, Code code)
    {
        Type type;

        switch (code)
        {
            case 0:
            case '\\':
            case ',':
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

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '9':
                type.set(kPath);
                switch (position)
                {
                    case 0:
                        break;
                    default:
                        type.set(kIdentifier);
                }
                break;

            case ';':
                switch (position)
                {
                    case 0:
                        type.set(kOperatorSemicolon);
                        break;
                }
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
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 3:
                        type |= Type(kKeywordCppProgramMask | kKeywordCppFileMask);
                        break;
                }
                break;

            case 'a':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 9:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'c':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 0:
                        type |= Type(kKeywordCppProgramMask | kKeywordCppFileMask);
                        break;
                }
                break;

            case 'e':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCppFileMask);
                        break;
                }
                break;
            case 'f':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCppFileMask);
                        break;
                }
                break;

            case 'i':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCppFileMask);
                        break;
                }
                break;

            case 'l':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type |= Type(kKeywordCppFileMask);
                        break;
                }
                break;

            case 'o':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'g':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;
            case 'm':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 10:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'r':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                    case 8:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            case 'p':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 1:
                    case 2:
                        type |= Type(kKeywordCppProgramMask | kKeywordCppFileMask);
                        break;
                    case 4:
                        type.set(kKeywordCppProgram);
                        break;
                }
                break;

            default:
                type |= Type(kIdentifierMask | kPathMask);
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
                type.set(kOperatorSemicolon, current[kOperatorSemicolon]);
                break;
            case 8:
                if (!isidentifier(code))
                    type.set(kKeywordCppFile, current[kKeywordCppFile]);
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
            case ':':
            case ';':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
                type.set(kWhiteSpace, current[kWhiteSpace]);
                type.set(kPath, current[kPath]);
                type.set(kIdentifier, current[kIdentifier]);
                break;

            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                type.set(kPath, current[kPath]);
                type.set(kIdentifier, current[kIdentifier]);
                break;

            default:
                type.set(kWhiteSpace, current[kWhiteSpace]);
        }

        return type;
    }
};
}
