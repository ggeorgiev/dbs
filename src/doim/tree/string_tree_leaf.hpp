//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tree/tree_leaf.hpp"

namespace doim
{
template <typename T, typename TN>
class StringTreeLeaf : public TreeLeaf<T, TN, string>
{
public:
    using TreeLeaf<T, TN, string>::TreeLeaf;

    const string& name() const
    {
        return std::get<1>(TreeLeaf<T, TN, string>::mArgs);
    }
};
}
