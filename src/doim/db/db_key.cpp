//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/db/db_key.h"
#include "doim/element.hpp"
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
    ASSERT(level < rankLevels());
    auto fn = [&ancestor, name, &key]() -> bool {
        key = DbKey::unique(ancestor, name);
        return true;
    };

    int rank = DbKey::rank() + level * im::InitializationManager::step();
    im::InitializationManager::subscribe<DbKeySPtr>(rank, fn, nullptr);
    return nullptr;
}
}
