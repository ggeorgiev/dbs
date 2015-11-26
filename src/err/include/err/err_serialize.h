#pragma once

#include "err/err_variadic.h"

#define EH_HELPER_SERIALIZE(X) EH_STRINGIZE(X) "(" << (X) << ")"
#define EH_HELPER_SERIALIZE_(X) ": " EH_STRINGIZE(X) " = " << (X)
#define EH_HELPER_SERIALIZE__(X) << ", " EH_STRINGIZE(X) " = " << (X)
#define EH_HELPER_SERIALIZE___(X) EH_HELPER_SERIALIZE__(X)
#define EH_HELPER_SERIALIZE____(X) EH_HELPER_SERIALIZE__(X)

#define EH_SERIALIZE(code, ...) EH_FOR_EACH(EH_HELPER_SERIALIZE, err::code, ##__VA_ARGS__) << "\n"
