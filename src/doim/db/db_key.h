//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <iosfwd>
#include <memory>
#include <string>
#include <tuple>

namespace doim
{
class DbKey;
typedef std::shared_ptr<DbKey> DbKeySPtr;

class DbKey : public Base<DbKey, DbKeySPtr, std::string>
{
public:
    static DbKeySPtr global(const std::string& name, DbKeySPtr& key);

    static DbKeySPtr global(const DbKeySPtr& ancestor,
                            int level,
                            const std::string& name,
                            DbKeySPtr& key);

    DbKey(const std::string& name);
    DbKey(const DbKeySPtr& ancestor, const std::string& name);

    const DbKeySPtr& ancestor() const
    {
        return std::get<0>(mArgs);
    }

    const std::string& name() const
    {
        return std::get<1>(mArgs);
    }

    std::string string() const;
};
}
