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
DbKeySPtr DbKey::global(const DbKeySPtr& ancestor,
                        int level,
                        const string& name,
                        DbKeySPtr& key)
{
    auto fn = [&ancestor, name, &key]() -> bool {
        key = DbKey::unique(ancestor, name);
        return true;
    };

    int rank = Manager::object_rank() + level * im::InitializationManager::step();
    im::InitializationManager::subscribe<DbKeySPtr>(rank, fn, nullptr);
    return nullptr;
}

DbKey::DbKey(const DbKeySPtr& ancestor, const string& name)
    : Base(ancestor, name)
{
    ASSERT(ancestor->isUnique());
}

string DbKey::toString() const
{
    if (ancestor() == nullptr)
        return name();
    return ancestor()->toString() + ":" + name();
}
}
