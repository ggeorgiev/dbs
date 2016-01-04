//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <bitset>
#include <string>

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
        kComment = 0,
        kIdentifier,
        kKeywordCxxBinary,
        kKeywordCxxFile,
        kKeywordCxxLibrary,
        kKeywordCxxProgram,
        kKeywordCxxPublicHeader,
        kOperatorAssignment,
        kOperatorAt,
        kOperatorColon,
        kOperatorSemicolon,
        kPath,
        kWhiteSpace,

        kCount
    };

    BITMASK0(kComment);
    BITMASK0(kIdentifier);
    BITMASK0(kKeywordCxxBinary);
    BITMASK0(kKeywordCxxFile);
    BITMASK0(kKeywordCxxLibrary);
    BITMASK0(kKeywordCxxProgram);
    BITMASK0(kKeywordCxxPublicHeader);
    BITMASK0(kOperatorAssignment);
    BITMASK0(kOperatorAt);
    BITMASK0(kOperatorColon);
    BITMASK0(kOperatorSemicolon);
    BITMASK0(kPath);
    BITMASK0(kWhiteSpace);

    typedef std::bitset<kCount> Type;

    static Type typeBody(const Type& current, size_t position, Code code)
    {
        Type type;
        if (current.test(kComment))
            type.set(kComment);

        switch (code)
        {
            case 0:
                type.set(kComment, false);
                break;

            case '\\':
            case ',':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
                break;

            case '\n':
            case '\r':
                type.set(kComment, false);
            //  no break;
            case ' ':
            case '\t':
            case '\v':
            case '\f':
                type |= Type(kWhiteSpaceMask);
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
                type |= Type(kPathMask);
                switch (position)
                {
                    case 0:
                        break;
                    default:
                        type |= Type(kIdentifierMask);
                }
                break;

            case '#':
                switch (position)
                {
                    case 0:
                        type |= Type(kCommentMask);
                        break;
                }
                break;

            case ';':
                switch (position)
                {
                    case 0:
                        type |= Type(kOperatorSemicolonMask);
                        break;
                }
                break;

            case ':':
                switch (position)
                {
                    case 0:
                        type |= Type(kOperatorColonMask);
                        break;
                }
                break;

            case '@':
                switch (position)
                {
                    case 0:
                        type |= Type(kOperatorAtMask);
                        break;
                }
                break;

            case '=':
                switch (position)
                {
                    case 0:
                        type |= Type(kOperatorAssignmentMask);
                        break;
                }
                break;

            case '_':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 3:
                        type |= Type(kKeywordCxxBinaryMask | kKeywordCxxFileMask |
                                     kKeywordCxxLibraryMask | kKeywordCxxProgramMask |
                                     kKeywordCxxPublicHeaderMask);
                        break;

                    case 10:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'a':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCxxBinaryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCxxLibraryMask);
                        break;
                    case 9:
                        type |= Type(kKeywordCxxProgramMask);
                        break;
                    case 13:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'b':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCxxBinaryMask);
                        break;
                    case 6:
                        type |=
                            Type(kKeywordCxxLibraryMask | kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'c':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 0:
                        type |= Type(kKeywordCxxBinaryMask | kKeywordCxxFileMask |
                                     kKeywordCxxLibraryMask | kKeywordCxxProgramMask |
                                     kKeywordCxxPublicHeaderMask);
                        break;
                    case 9:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;
            case 'd':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 14:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;
            case 'e':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCxxFileMask);
                        break;
                    case 12:
                    case 15:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;
            case 'f':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCxxFileMask);
                        break;
                }
                break;

            case 'g':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCxxProgramMask);
                        break;
                }
                break;

            case 'h':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 11:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'i':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCxxBinaryMask | kKeywordCxxFileMask |
                                     kKeywordCxxLibraryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'l':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCxxLibraryMask);
                        break;
                    case 6:
                        type |= Type(kKeywordCxxFileMask);
                        break;
                    case 7:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'm':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 10:
                        type |= Type(kKeywordCxxProgramMask);
                        break;
                }
                break;

            case 'n':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type |= Type(kKeywordCxxBinaryMask);
                        break;
                }
                break;

            case 'o':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type |= Type(kKeywordCxxProgramMask);
                        break;
                }
                break;

            case 'p':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |=
                            Type(kKeywordCxxProgramMask | kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'r':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCxxProgramMask);
                        break;
                    case 7:
                        type |= Type(kKeywordCxxLibraryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCxxBinaryMask | kKeywordCxxProgramMask);
                        break;
                    case 9:
                        type |= Type(kKeywordCxxLibraryMask);
                        break;
                    case 16:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'u':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCxxPublicHeaderMask);
                        break;
                }
                break;

            case 'x':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 1:
                    case 2:
                        type |= Type(kKeywordCxxBinaryMask | kKeywordCxxFileMask |
                                     kKeywordCxxLibraryMask | kKeywordCxxProgramMask |
                                     kKeywordCxxPublicHeaderMask);
                        break;
                }

            case 'y':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 9:
                        type |= Type(kKeywordCxxBinaryMask);
                        break;
                    case 10:
                        type |= Type(kKeywordCxxLibraryMask);
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
                type.set(kOperatorAssignment, current[kOperatorAssignment]);
                type.set(kOperatorAt, current[kOperatorAt]);
                type.set(kOperatorColon, current[kOperatorColon]);
                type.set(kOperatorSemicolon, current[kOperatorSemicolon]);
                break;
            case 8:
                if (!isidentifier(code))
                    type.set(kKeywordCxxFile, current[kKeywordCxxFile]);
                break;
            case 10:
                if (!isidentifier(code))
                    type.set(kKeywordCxxBinary, current[kKeywordCxxBinary]);
                break;
            case 11:
                if (!isidentifier(code))
                {
                    type.set(kKeywordCxxLibrary, current[kKeywordCxxLibrary]);
                    type.set(kKeywordCxxProgram, current[kKeywordCxxProgram]);
                }
                break;
            case 17:
                if (!isidentifier(code))
                    type.set(kKeywordCxxPublicHeader, current[kKeywordCxxPublicHeader]);
                break;
        }

        switch (code)
        {
            case 0:
                type.set(kComment, current[kComment]);
            //  no break;
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
            case '#':
            case '=':
                type.set(kWhiteSpace, current[kWhiteSpace]);
                type.set(kPath, current[kPath]);
                type.set(kIdentifier, current[kIdentifier]);
                break;

            case '\r':
            case '\n':
                type.set(kComment, current[kComment]);
            //  no break;
            case ' ':
            case '\t':
            case '\v':
            case '\f':
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
