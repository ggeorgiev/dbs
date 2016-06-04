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
    static int constexpr rankLevels()
    {
        return 2;
    }

    using StringTreeNode<DbKey, ':'>::global;
    static DbKeySPtr global(const DbKeySPtr& ancestor,
                            int level,
                            const string& name,
                            DbKeySPtr& key);

    using StringTreeNode<DbKey, ':'>::StringTreeNode;
};
}
