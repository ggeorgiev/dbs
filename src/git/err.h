//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"

#define EHGitTest(expression, ...)                                            \
    do                                                                        \
    {                                                                         \
        int err = (expression);                                               \
        if (err < 0)                                                          \
        {                                                                     \
            auto git_err = giterr_last();                                     \
            if (git_err != nullptr)                                           \
                EHBan(kGit, git_err->klass, git_err->message, ##__VA_ARGS__); \
            else                                                              \
                EHBan(kGit, err, ##__VA_ARGS__);                              \
        }                                                                     \
    } while (false)
