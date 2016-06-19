//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/prj/depository.h"
#include "err/err.h"
#include "log/log.h"
#include "log/log_function.hpp"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class DepositoryMixin
{
public:
    typedef T Subject;

    ECode updateDepository(const dom::DepositorySPtr& depository)
    {
        mDepository = depository;
        EHEnd;
    }

private:
    dom::DepositorySPtr mDepository;
};
}
