//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_relative_directory.h"
#include "doim/tree/string_tree_leaf.hpp"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <string_view>
#include <tuple>

namespace doim
{
class FsRelativeFile;
typedef shared_ptr<FsRelativeFile> FsRelativeFileSPtr;

class FsRelativeFile : public StringTreeLeaf<FsRelativeFile, FsRelativeDirectory>
{
public:
    using StringTreeLeaf<FsRelativeFile, FsRelativeDirectory>::StringTreeLeaf;

    auto path() const
    {
        return StringTreeLeaf<FsRelativeFile, FsRelativeDirectory>::toString();
    }
};
}
