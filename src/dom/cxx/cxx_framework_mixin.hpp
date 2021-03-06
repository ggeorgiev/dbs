//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_framework.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "log/log.h"
#include "log/log_function.hpp"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class CxxFrameworkMixin
{
public:
    typedef T Subject;

    doim::CxxFrameworkSet frameworks()
    {
        return mFrameworkList;
    }

    ECode updateFrameworkList(doim::CxxFrameworkSet& frameworks)
    {
        mFrameworkList.swap(frameworks);
        EHEnd;
    }

private:
    doim::CxxFrameworkSet mFrameworkList;
};
}
