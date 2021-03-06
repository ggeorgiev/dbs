//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err_variadic.h"
#include <fmt/format.h>

#define EH_HELPER_CPPFORMAT(X) PP_STRINGIZE(X) "({})"
#define EH_HELPER_CPPFORMAT_(X) ": " PP_STRINGIZE(X) " = {}"
#define EH_HELPER_CPPFORMAT__(X) ", " PP_STRINGIZE(X) " = {}"
#define EH_HELPER_CPPFORMAT___(X) EH_HELPER_CPPFORMAT__(X)
#define EH_HELPER_CPPFORMAT____(X) EH_HELPER_CPPFORMAT__(X)

#define EH_CPPFORMAT(code, ...)                                             \
    fmt::format(EH_FOR_EACH(EH_HELPER_CPPFORMAT, code, ##__VA_ARGS__) "\n", \
                err::code,                                                  \
                ##__VA_ARGS__)

#define EH_CPPFORMAT_CONTEXT(expression, ...)                                     \
    fmt::format(EH_FOR_EACH(EH_HELPER_CPPFORMAT, expression, ##__VA_ARGS__) "\n", \
                expression,                                                       \
                ##__VA_ARGS__)
