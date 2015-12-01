#pragma once

/*
 * Err is an error handling system that allows call stack tracking.
 */

#include "err/err_assert.h"
#include "err/err_cppformat.h"

#include <stddef.h>
#include <sstream>
#include <vector>

namespace err
{
enum ECode
{
    kSuccess = 0,

    // Error handling system codes
    kExpected = 1,
    kAssert = 2,

    kUnable = 10,
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
    Error(ECode code, const Location& location) : mCode(code) { mCallstack.push_back(location); }
    ECode code() { return mCode; }
    std::string message() { return mMessage.str(); }
    std::stringstream& message_stream() { return mMessage; }
    void addLocation(const Location& location) { mCallstack.push_back(location); }
    std::string callstack()
    {
        std::stringstream stream;
        for (const auto& location : mCallstack)
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
}

using err::ECode;

#define EH_CODE(code, ...) err::code

#define EH_LOCATION err::Location(__FILE__, __LINE__, __FUNCTION__)

#define EHEnd return err::kSuccess

#define EHBan(...)                                                                   \
    do                                                                               \
    {                                                                                \
        ECode __EHBan__code = EH_CODE(__VA_ARGS__);                                  \
        (err::gError = new err::Error(__EHBan__code, EH_LOCATION))->message_stream() \
            << EH_CPPFORMAT(__VA_ARGS__);                                            \
        return __EHBan__code;                                                        \
    } while (false)

#define EHTest(expression, ...)                                                             \
    do                                                                                      \
    {                                                                                       \
        ECode preserve = (expression);                                                      \
        if (preserve != err::kSuccess)                                                      \
        {                                                                                   \
            err::gError->message_stream() << EH_CPPFORMAT_CONTEXT(preserve, ##__VA_ARGS__); \
            err::gError->addLocation(EH_LOCATION);                                          \
            return preserve;                                                                \
        }                                                                                   \
    } while (false)

#define EHReset                                      \
    do                                               \
    {                                                \
        ASSERT(err::gError != nullptr);              \
        ASSERT(err::gError->code() != err::kAssert); \
        delete err::gError;                          \
        err::gError = NULL;                          \
    } while (false)

#if defined(NDEBUG)
#define EHAssert(X) ASSERT(X)
#else
#define EHAssert(X)            \
    do                         \
    {                          \
        bool expression = (X); \
        if (!expression)       \
            EHBan(kAssert, X); \
    } while (false)
#endif
