#pragma once

/*
 * Err is an error handling system that allows call stack tracking.
 */

#include <sstream>

enum ECode
{
    kSuccess = 0,

    kError = 1,
};

class Error
{
public:
    Error(ECode code)
        : mCode(code)
    {
    }

    std::stringstream& message()
    {
        return mMessage;
    }

    std::stringstream& callstack()
    {
        return mCallstack;
    }

private:
    ECode mCode;
    std::stringstream mMessage;
    std::stringstream mCallstack;
};

typedef Error* ErrorRPtr;

thread_local ErrorRPtr gError;

#define EH_STRINGIZE(arg) EH_STRINGIZE1(arg)
#define EH_STRINGIZE1(arg) EH_STRINGIZE2(arg)
#define EH_STRINGIZE2(arg) #arg

// Make a FOREACH macro
#define EHFE_1(WHAT, X) WHAT(X) << "\n"
#define EHFE_2(WHAT, X, ...) WHAT(X) EHFE_1(WHAT##_, __VA_ARGS__)
#define EHFE_3(WHAT, X, ...) WHAT(X) EHFE_2(WHAT##_, __VA_ARGS__)
#define EHFE_4(WHAT, X, ...) WHAT(X) EHFE_3(WHAT##_, __VA_ARGS__)
#define EHFE_5(WHAT, X, ...) WHAT(X) EHFE_4(WHAT##_, __VA_ARGS__)
//... repeat as needed

#define EH_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define EH_FOR_EACH(action, ...) \
    EH_GET_MACRO(__VA_ARGS__, EHFE_5, EHFE_4, EHFE_3, EHFE_2, EHFE_1, )(action, __VA_ARGS__)

// Example
// Some actions
#define EH_HELPER_SERIALIZE(X) EH_STRINGIZE(X) "(" << (X) << ")"
#define EH_HELPER_SERIALIZE_(X) ": " EH_STRINGIZE(X) " = " << (X)
#define EH_HELPER_SERIALIZE__(X) << ", " EH_STRINGIZE(X) " = " << (X)
#define EH_HELPER_SERIALIZE___(X) EH_HELPER_SERIALIZE__(X)
#define EH_HELPER_SERIALIZE____(X) EH_HELPER_SERIALIZE__(X)

#define EH_SERIALIZE(...) EH_FOR_EACH(EH_HELPER_SERIALIZE, __VA_ARGS__)

#define EH_CODE(code, ...) code

#define EHRET(...)                                  \
    ECode __EHRET__code = EH_CODE(__VA_ARGS__);     \
    gError = new Error(__EHRET__code);              \
    gError->message() << EH_SERIALIZE(__VA_ARGS__); \
    std::cout << gError->message().str();           \
    return __EHRET__code
