//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "option/verbose.h"
#include "doim/tag/tag.h"
#include "log/log.h"
#include "math/crc.hpp"
#include "rtti/class_rtti.hpp"
#include <boost/functional/hash.hpp>
#include <functional>
#include <memory>
#include <shared_ptr>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace task
{
struct ICrcTask
{
    virtual math::Crcsum crc() = 0;
    virtual ECode crc(math::Crcsum& crcsum) = 0;
};

typedef shared_ptr<ICrcTask> ICrcTaskSPtr;

template <typename T, typename... Args>
class CrcTask : public ICrcTask, public Element<T, Args...>
{
public:
    using Element<T, Args...>::Element;

    virtual math::Crcsum crc() override
    {
        return mCrcsum;
    }

    ECode crc(math::Crcsum& crcsum) override
    {
        EHTest(tpool::Task::join());
        crcsum = mCrcsum;
        EHEnd;
    }

protected:
    math::Crcsum mCrcsum;
};
}
