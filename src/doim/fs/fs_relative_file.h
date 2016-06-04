//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_relative_directory.h"
#include "doim/tree/tree_leaf.hpp"
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

class FsRelativeFile : public TreeLeaf<FsRelativeFile, FsRelativeDirectorySPtr, string>
{
public:
    using TreeLeaf<FsRelativeFile, FsRelativeDirectorySPtr, string>::find;

    FsRelativeFile(const FsRelativeDirectorySPtr& directory, const string& name);
};
}
