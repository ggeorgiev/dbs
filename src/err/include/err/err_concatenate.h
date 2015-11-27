#pragma once

#include "err/err_variadic.h"

#include <boost/lexical_cast.hpp>

#define EH_STR(X) boost::lexical_cast<std::string>(X)
#define EH_HELPER_CONCATENATE(X) EH_STRINGIZE(X) "(" + EH_STR(X) + ")"
#define EH_HELPER_CONCATENATE_(X) ": " EH_STRINGIZE(X) " = " + EH_STR(X)
#define EH_HELPER_CONCATENATE__(X) +", " EH_STRINGIZE(X) " = " + EH_STR(X)
#define EH_HELPER_CONCATENATE___(X) EH_HELPER_CONCATENATE__(X)
#define EH_HELPER_CONCATENATE____(X) EH_HELPER_CONCATENATE__(X)

#define EH_CONCATENATE(code, ...) \
    EH_FOR_EACH(EH_HELPER_CONCATENATE, err::code, ##__VA_ARGS__) + "\n"
