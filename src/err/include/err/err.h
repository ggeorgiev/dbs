//  Copyright © 2015 George Georgiev. All rights reserved.
//
//
// Err is an error handling system that allows call stack tracking.
//

#pragma once

#include "err/err_assert.h"
#include "err/err_cppformat.h"
#include "err/pparg.h"
#include "im/initialization_manager.hpp"
#include <sstream>
#include <stddef.h>
#include <unordered_map>
#include <vector>

namespace err
{
inline int initialization_rank()
{
    return im::InitializationManager::rank_base() +
           im::InitializationManager::rank_step();
}

enum ECode
{
    kSuccess = 0,

    // Error handling system codes
    kExpected = 1,
    kAssert = 2,

    kNotFound = 10,
    kTooMany = 11,
    kUnable = 12,
};

struct EnumHasher
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

typedef std::unordered_map<ECode, const char*, EnumHasher> NameMap;

extern NameMap gNames;

inline const char* name(ECode code)
{
    ASSERT(gNames.count(code) > 0);
    return gNames[code];
}

struct Location
{
    Location(const char* file, size_t line, const char* function)
        : mFile(file)
        , mLine(line)
        , mFunction(function)
    {
    }

    const char* mFile;
    size_t mLine;
    const char* mFunction;
};

class Error
{
public:
    Error(ECode code, const Location& location)
        : mCode(code)
    {
        mCallstack.push_back(location);
    }

    ECode code()
    {
        return mCode;
    }

    std::string message()
    {
        return mMessage.str();
    }

    std::stringstream& message_stream()
    {
        return mMessage;
    }

    void addLocation(const Location& location)
    {
        mCallstack.push_back(location);
    }

    std::string callstack()
    {
        std::stringstream stream;
        for (const auto& location : mCallstack)
        {
            stream << location.mFile << ":" << location.mLine << ": @ "
                   << location.mFunction << "\n";
        }
        return stream.str();
    }

private:
    ECode mCode;
    std::stringstream mMessage;
    std::vector<Location> mCallstack;
};

typedef Error* ErrorRPtr;
typedef std::unique_ptr<Error> ErrorUPtr;

extern thread_local ErrorUPtr gError;
}

using err::ECode;

#define EH_CODE(code, ...) err::code

#define EH_LOCATION \
    err::Location(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__))

#define EHEnd return err::kSuccess

#define EHBan(...)                                                         \
    do                                                                     \
    {                                                                      \
        ASSERT(err::gError == nullptr);                                    \
        ECode __EHBan__code = EH_CODE(__VA_ARGS__);                        \
        err::ErrorRPtr error = new err::Error(__EHBan__code, EH_LOCATION); \
        if (PP_NARG(__VA_ARGS__) > 0)                                      \
        {                                                                  \
            error->message_stream() << EH_CPPFORMAT(__VA_ARGS__);          \
        }                                                                  \
        err::gError.reset(error);                                          \
        return __EHBan__code;                                              \
    } while (false)

#define EHTest(expression, ...)                                       \
    do                                                                \
    {                                                                 \
        ECode preserve = (expression);                                \
        if (preserve != err::kSuccess)                                \
        {                                                             \
            if (PP_NARG(__VA_ARGS__) > 0)                             \
            {                                                         \
                err::gError->message_stream()                         \
                    << EH_CPPFORMAT_CONTEXT(preserve, ##__VA_ARGS__); \
            }                                                         \
            err::gError->addLocation(EH_LOCATION);                    \
            return preserve;                                          \
        }                                                             \
    } while (false)

#define EHReset                                      \
    do                                               \
    {                                                \
        ASSERT(err::gError != nullptr);              \
        ASSERT(err::gError->code() != err::kAssert); \
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
