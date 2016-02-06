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
class DbKey;
typedef std::shared_ptr<DbKey> DbKeySPtr;

class DbKey
{
public:
    DbKey(const std::string& bytes)
        : mBytes(bytes)
    {
    }

    const std::string& bytes() const
    {
        return mBytes;
    }

    struct Hasher
    {
        std::size_t operator()(const DbKeySPtr& key) const
        {
            return hashString(key->bytes());
        }

        bool operator()(const DbKeySPtr& key1, const DbKeySPtr& key2) const
        {
            return key1->bytes() == key2->bytes();
        }

        std::hash<std::string> hashString;
    };

private:
    std::string mBytes;
};
}
