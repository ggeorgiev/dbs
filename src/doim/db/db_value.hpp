//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <str>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class DbValue;
typedef shared_ptr<DbValue> DbValueSPtr;

class DbValue : public Element<DbValue, string>
{
public:
    template <typename T>
    DbValue(const T& obj)
        : Element(string(reinterpret_cast<const char*>(&obj), sizeof(obj)))
    {
    }

    const string& bytes() const
    {
        return std::get<0>(mArgs);
    }
};

template <>
inline DbValue::DbValue(const string& bytes)
    : Element(bytes)
{
}
}
