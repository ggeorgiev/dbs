//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//
//
// Err is an error handling system that allows call stack tracking.
//

#pragma once

#include "err/err_assert.h"
#include "err/err_cppformat.h" // IWYU pragma: export
#include "err/macro.h"         // IWYU pragma: export
#include "im/initialization_manager.hpp"
#include <fmt/format.h>
#include <boost/thread/tss.hpp> // IWYU pragma: export
#include <memory>
#include <str>
#include <unordered>
#include <vector>
#include <stddef.h>

namespace err
{
constexpr int initialization_rank()
{
    return im::InitializationManager::rank_base() +
           im::InitializationManager::rank_step();
}

enum ECode
{
    kSuccess = 0,

    // Error handling system codes
    kExpected = 1,
    kAssert,

    // Action error codes
    kFailed = 10,
    kNotFound,
    kTooMany,
    kUnable,
    kUnexpected,
    kUnknown,

    // Subsystems error codes
    kDatabase = 20,
    kFileSystem,
};

struct EnumHasher
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

typedef unordered_map<ECode, const char*, EnumHasher> NameMap;

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

    Error(const Error& error)
        : mCode(error.code())
        , mMessage(error.message())
        , mCallstack(error.mCallstack)
    {
    }

    ECode code() const
    {
        return mCode;
    }

    string message() const
    {
        return mMessage.str();
    }

    stringstream& message_stream()
    {
        return mMessage;
    }

    void addLocation(const Location& location)
    {
        mCallstack.push_back(location);
    }

    string callstack()
    {
        string stack;
        for (const auto& location : mCallstack)
        {
            stack += fmt::format("{}:{}: @ {}\n",
                                 location.mFile,
                                 location.mLine,
                                 location.mFunction);
        }
        return stack;
    }

private:
    ECode mCode;
    stringstream mMessage;
    std::vector<Location> mCallstack;
};

template <typename T>
class Ptr
{
public:
    Ptr(const T& p)
        : mP(p)
    {
    }

    const T& mP;
};

template <typename T>
Ptr<T> ptr(const T& p)
{
    return Ptr<T>(p);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Ptr<T>& p)
{
    if (p.mP == nullptr)
        out << "null";
    else
        out << *p.mP;
    return out;
}

typedef Error* ErrorRPtr;
typedef std::unique_ptr<Error> ErrorUPtr;

extern boost::thread_specific_ptr<Error> gError;
}

using err::ECode;

#define EHPtr(x) err::ptr(x)

#define EH_CODE(code, ...) err::code

#define EH_LOCATION \
    err::Location(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__))

#define EHEnd return err::kSuccess

#define EHBan(...)                              \
    {                                           \
        EHAssert(err::gError.get() == nullptr); \
        EHBan_(__VA_ARGS__);                    \
    }

#define EHBan_(...)                                               \
    {                                                             \
        auto __EHBan__code = EH_CODE(__VA_ARGS__);                \
        auto error = new err::Error(__EHBan__code, EH_LOCATION);  \
        if (PP_NARG(__VA_ARGS__) > 0)                             \
            error->message_stream() << EH_CPPFORMAT(__VA_ARGS__); \
        err::gError.reset(error);                                 \
        return __EHBan__code;                                     \
    }

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

#define EHReset                                        \
    do                                                 \
    {                                                  \
        EHAssert(err::gError != nullptr);              \
        EHAssert(err::gError->code() != err::kAssert); \
        err::gError.reset();                           \
    } while (false)

#define EHEnsureClear err::gError.reset();

// EHAssert is similar to ASSERT, but it can be used only from functions that return
// ECode. The benefit over ASSERT is that it will construct the call stack that will
// simplify triaging.
#if defined(NDEBUG)
#define EHAssert(X, ...) ASSERT(X)
#else
#define EHAssert(X, ...)                       \
    do                                         \
    {                                          \
        bool expression = (X);                 \
        if (!expression)                       \
            EHBan_(kAssert, X, ##__VA_ARGS__); \
    } while (false)
#endif
