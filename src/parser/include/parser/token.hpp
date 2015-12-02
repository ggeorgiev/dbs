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
        kComment = 0,
        kIdentifier,
        kKeywordCppBinary,
        kKeywordCppFile,
        kKeywordCppLibrary,
        kKeywordCppProgram,
        kKeywordCppPublicDirectory,
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
    BITMASK0(kKeywordCppBinary);
    BITMASK0(kKeywordCppFile);
    BITMASK0(kKeywordCppLibrary);
    BITMASK0(kKeywordCppProgram);
    BITMASK0(kKeywordCppPublicDirectory);
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
                        type |= Type(kKeywordCppBinaryMask | kKeywordCppFileMask |
                                     kKeywordCppLibraryMask | kKeywordCppProgramMask |
                                     kKeywordCppPublicDirectoryMask);
                        break;

                    case 10:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'a':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCppBinaryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCppLibraryMask);
                        break;
                    case 9:
                        type |= Type(kKeywordCppProgramMask);
                        break;
                }
                break;

            case 'b':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCppBinaryMask);
                        break;
                    case 6:
                        type |= Type(kKeywordCppLibraryMask | kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'c':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 0:
                        type |= Type(kKeywordCppBinaryMask | kKeywordCppFileMask |
                                     kKeywordCppLibraryMask | kKeywordCppProgramMask |
                                     kKeywordCppPublicDirectoryMask);
                        break;
                    case 9:
                    case 15:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;
            case 'd':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 11:
                        type |= Type(kKeywordCppPublicDirectoryMask);
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
                    case 14:
                        type |= Type(kKeywordCppPublicDirectoryMask);
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

            case 'g':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 7:
                        type |= Type(kKeywordCppProgramMask);
                        break;
                }
                break;

            case 'i':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCppBinaryMask | kKeywordCppFileMask |
                                     kKeywordCppLibraryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                    case 12:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'l':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 4:
                        type |= Type(kKeywordCppLibraryMask);
                        break;
                    case 6:
                        type |= Type(kKeywordCppFileMask);
                        break;
                    case 7:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'm':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 10:
                        type |= Type(kKeywordCppProgramMask);
                        break;
                }
                break;

            case 'n':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type |= Type(kKeywordCppBinaryMask);
                        break;
                }
                break;

            case 'o':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 6:
                        type |= Type(kKeywordCppProgramMask);
                        break;
                    case 17:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'p':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 1:
                    case 2:
                        type |= Type(kKeywordCppBinaryMask | kKeywordCppFileMask |
                                     kKeywordCppLibraryMask | kKeywordCppProgramMask |
                                     kKeywordCppPublicDirectoryMask);
                        break;
                    case 4:
                        type |= Type(kKeywordCppProgramMask | kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'r':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCppProgramMask);
                        break;
                    case 7:
                        type |= Type(kKeywordCppLibraryMask);
                        break;
                    case 8:
                        type |= Type(kKeywordCppBinaryMask | kKeywordCppProgramMask);
                        break;
                    case 9:
                        type |= Type(kKeywordCppLibraryMask);
                        break;

                    case 13:
                    case 18:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 't':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 16:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'u':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 5:
                        type |= Type(kKeywordCppPublicDirectoryMask);
                        break;
                }
                break;

            case 'y':
                type |= Type(kIdentifierMask | kPathMask);
                switch (position)
                {
                    case 9:
                        type |= Type(kKeywordCppBinaryMask);
                        break;
                    case 10:
                        type |= Type(kKeywordCppLibraryMask);
                        break;
                    case 19:
                        type |= Type(kKeywordCppPublicDirectoryMask);
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
                    type.set(kKeywordCppFile, current[kKeywordCppFile]);
                break;
            case 10:
                if (!isidentifier(code))
                    type.set(kKeywordCppBinary, current[kKeywordCppBinary]);
                break;
            case 11:
                if (!isidentifier(code))
                {
                    type.set(kKeywordCppLibrary, current[kKeywordCppLibrary]);
                    type.set(kKeywordCppProgram, current[kKeywordCppProgram]);
                }
                break;
            case 20:
                if (!isidentifier(code))
                {
                    type.set(kKeywordCppPublicDirectory, current[kKeywordCppPublicDirectory]);
                }
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
