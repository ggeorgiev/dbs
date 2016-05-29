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
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
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
template <typename T, typename... Args>
class CrcTask : public Element<T, Args...>
{
public:
    using Element<T, Args...>::Element;

    math::Crcsum crc()
    {
        return mCrcsum;
    }

protected:
    math::Crcsum mCrcsum;
};
}
