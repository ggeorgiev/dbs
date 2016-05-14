//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class DbKey;
typedef shared_ptr<DbKey> DbKeySPtr;

class DbKey : public Base<DbKey, DbKeySPtr, string>
{
public:
    static DbKeySPtr global(const string& name, DbKeySPtr& key);

    static DbKeySPtr global(const DbKeySPtr& ancestor,
                            int level,
                            const string& name,
                            DbKeySPtr& key);

    DbKey(const string& name);
    DbKey(const DbKeySPtr& ancestor, const string& name);

    const DbKeySPtr& ancestor() const
    {
        return std::get<0>(mArgs);
    }

    const string& name() const
    {
        return std::get<1>(mArgs);
    }

    string toString() const;
};
}
