//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/db/db_key.h"
#include "doim/base.hpp"
#include "doim/manager.h"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
DbKeySPtr DbKey::global(const DbKeySPtr& ancestor,
                        const std::string& name,
                        DbKeySPtr& key)
{
    return Manager::global<DbKey, DbKeySPtr, std::string>(ancestor, name, key);
}

DbKey::DbKey(const std::string& name)
    : Base(nullptr, name)
{
}
DbKey::DbKey(const DbKeySPtr& ancestor, const std::string& name)
    : Base(ancestor, name)
{
    // TODO: enablee this assert when global initialization is fixed
    // ASSERT(gManager->isUnique(ancestor);
}

std::string DbKey::string() const
{
    if (ancestor() == nullptr)
    {
        return name();
    }
    return ancestor()->string() + ":" + name();
}
}
