#pragma once

/*
 * Err is an error handling system that allows call stack tracking.
 */

#include "err/err_cppformat.h"

#include <sstream>
#include <vector>

enum ECode
{
    kSuccess = 0,

    kExpected = 1,
};

struct Location
{
    Location(const char* file, size_t line, const char* function)
        : mFile(file), mLine(line), mFunction(function)
    {
    }

    const char* mFile;
    size_t mLine;
    const char* mFunction;
};

class Error
{
public:
    Error(ECode code, Location location) : mCode(code) { mCallstack.push_back(location); }
    std::string message() { return mMessage.str(); }
    std::stringstream& message_stream() { return mMessage; }
    std::string callstack()
    {
        std::stringstream stream;
        for (auto location : mCallstack)
        {
            stream << location.mFile << ":" << location.mLine << ": @ " << location.mFunction
                   << "\n";
        }
        return stream.str();
    }

private:
    ECode mCode;
    std::stringstream mMessage;
    std::vector<Location> mCallstack;
};

typedef Error* ErrorRPtr;

extern thread_local ErrorRPtr gError;

#define EH_CODE(code, ...) code

#define EH_LOCATION Location(__FILE__, __LINE__, __FUNCTION__)

#define EHRET(...)                                                     \
    ECode __EHRET__code = EH_CODE(__VA_ARGS__);                        \
    (gError = new Error(__EHRET__code, EH_LOCATION))->message_stream() \
        << EH_CPPFORMAT(__VA_ARGS__);                          \
    return __EHRET__code

#define EH_RESET   \
    delete gError; \
    gError = NULL;
