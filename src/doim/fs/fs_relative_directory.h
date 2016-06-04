//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tree/tree_node.hpp"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <string_view>
#include <tuple>
#include <stddef.h>

namespace doim
{
class FsRelativeDirectory;
typedef shared_ptr<FsRelativeDirectory> FsRelativeDirectorySPtr;

class FsRelativeDirectory : public TreeNode<FsRelativeDirectory, string>
{
public:
    using TreeNode<FsRelativeDirectory, string>::find;

    FsRelativeDirectory();
    FsRelativeDirectory(const FsRelativeDirectorySPtr& parent, const string& name);

    const string& name() const
    {
        return std::get<1>(mArgs);
    }
};
}
