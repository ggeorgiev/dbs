//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tree/tree_node.hpp"
#include "err/err_assert.h"
#include <str>

namespace doim
{
template <typename T, char D>
class StringTreeNode : public TreeNode<T, string>
{
public:
    static char constexpr delimiter()
    {
        return D;
    }

    using TreeNode<T, string>::TreeNode;

    using TreeNode<T, string>::ancestor;

    const string& name() const
    {
        return std::get<1>(TreeNode<T, string>::mArgs);
    }

    string toString() const
    {
        if (ancestor() == nullptr)
            return name() + delimiter();
        return ancestor()->toString() + name() + delimiter();
    }
};
}
