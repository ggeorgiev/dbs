//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class DbValue;
typedef std::shared_ptr<DbValue> DbValueSPtr;

class DbValue : public Base<DbValue, std::string>
{
public:
    template <typename T>
    DbValue(const T& obj)
        : Base(std::string(reinterpret_cast<const char*>(&obj), sizeof(obj)))
    {
    }

    const std::string& bytes() const
    {
        return std::get<0>(mArgs);
    }
};

template <>
inline DbValue::DbValue(const std::string& bytes)
    : Base(bytes)
{
}
}
