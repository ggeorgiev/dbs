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

    const string& name() const
    {
        return std::get<1>(TreeNode<T, string>::mArgs);
    }

    string toString() const
    {
        if (TreeNode<T, string>::ancestor() == nullptr)
            return name();
        return TreeNode<T, string>::ancestor()->toString() + delimiter() + name();
    }
};
}
