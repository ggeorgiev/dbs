//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#define PP_STRINGIZE(arg) PP_STRINGIZE1(arg)
#define PP_STRINGIZE1(arg) PP_STRINGIZE2(arg)
#define PP_STRINGIZE2(arg) #arg

#define PP_NARG(...) PP_NARG_(a, ##__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define PP_RSEQ_N() 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, _
