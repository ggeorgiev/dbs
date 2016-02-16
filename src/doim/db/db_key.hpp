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
class DbKey;
typedef std::shared_ptr<DbKey> DbKeySPtr;

class DbKey : public Base<DbKey, std::string>
{
public:
    DbKey(const std::string& bytes)
        : Base(bytes)
    {
    }

    const std::string& bytes() const
    {
        return std::get<0>(mArgs);
    }
};
}
