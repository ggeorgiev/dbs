//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tree/string_tree_node.hpp"
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

class FsRelativeDirectory : public StringTreeNode<FsRelativeDirectory, '/'>
{
public:
    using StringTreeNode<FsRelativeDirectory, '/'>::StringTreeNode;
};
}
