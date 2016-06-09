//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tree/string_tree_node.hpp"
#include "doim/element.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class DbKey;
typedef shared_ptr<DbKey> DbKeySPtr;

class DbKey : public StringTreeNode<DbKey, ':'>
{
public:
    static int constexpr levels()
    {
        return 2;
    }

    using StringTreeNode<DbKey, ':'>::global;

    template <int N = 1>
    static DbKeySPtr global(const DbKeySPtr& ancestor, const string& name, DbKeySPtr& key)
    {
        static_assert(N <= levels(),
                      "Initialization with level that is higher than the declared.");
        auto fn = [&ancestor, name, &key]() -> bool {
            key = DbKey::unique(ancestor, name);
            return true;
        };

        int rank = DbKey::rank() + N * im::InitializationManager::step();
        im::InitializationManager::subscribe<DbKeySPtr>(rank, fn, nullptr);
        return nullptr;
    }

    using StringTreeNode<DbKey, ':'>::StringTreeNode;
};
}
