//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/db/db_key.h"
#include "doim/base.hpp"
#include "doim/manager.h"
#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include <functional>
#include <memory>
#include <str>

namespace doim
{
DbKeySPtr DbKey::global(const string& name, DbKeySPtr& key)
{
    return Manager::global<DbKey, DbKeySPtr, string>(nullptr, name, key);
}

DbKeySPtr DbKey::global(const DbKeySPtr& ancestor,
                        int level,
                        const string& name,
                        DbKeySPtr& key)
{
    auto fn = [&ancestor, name, &key]() -> bool {
        key = doim::unique<DbKey>(ancestor, name);
        return true;
    };

    int rank = Manager::object_rank() + level * im::InitializationManager::step();
    im::InitializationManager::subscribe<DbKeySPtr>(rank, fn, nullptr);
    return nullptr;
}

DbKey::DbKey(const string& name)
    : Base(nullptr, name)
{
}
DbKey::DbKey(const DbKeySPtr& ancestor, const string& name)
    : Base(ancestor, name)
{
    ASSERT(gManager->isUnique(ancestor));
}

string DbKey::toString() const
{
    if (ancestor() == nullptr)
    {
        return name();
    }
    return ancestor()->toString() + ":" + name();
}
}
