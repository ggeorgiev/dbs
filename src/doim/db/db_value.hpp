//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

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

class DbValue
{
public:
    template <typename T>
    DbValue(const T& obj)
        : mBytes(reinterpret_cast<const char*>(&obj), sizeof(obj))
    {
    }

    const std::string& bytes() const
    {
        return mBytes;
    }

    struct Hasher
    {
        std::size_t operator()(const DbValueSPtr& value) const
        {
            return hashString(value->bytes());
        }

        bool operator()(const DbValueSPtr& value1, const DbValueSPtr& value2) const
        {
            return value1->bytes() == value2->bytes();
        }

        std::hash<std::string> hashString;
    };

private:
    std::string mBytes;
};

template <>
inline DbValue::DbValue(const std::string& bytes)
    : mBytes(bytes)
{
}
}
