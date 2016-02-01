#pragma once

#define EH_FE_1(WHAT, X) WHAT(X)
#define EH_FE_2(WHAT, X, ...) WHAT(X) EH_FE_1(WHAT##_, __VA_ARGS__)
#define EH_FE_3(WHAT, X, ...) WHAT(X) EH_FE_2(WHAT##_, __VA_ARGS__)
#define EH_FE_4(WHAT, X, ...) WHAT(X) EH_FE_3(WHAT##_, __VA_ARGS__)
#define EH_FE_5(WHAT, X, ...) WHAT(X) EH_FE_4(WHAT##_, __VA_ARGS__)

#define EH_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define EH_FOR_EACH(action, ...)                                             \
    EH_GET_MACRO(__VA_ARGS__, EH_FE_5, EH_FE_4, EH_FE_3, EH_FE_2, EH_FE_1, ) \
    (action, __VA_ARGS__)
