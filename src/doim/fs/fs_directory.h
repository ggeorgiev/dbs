//  Copyright © 2015-2016 George Georgiev. All rights reserved.
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
class FsDirectory;
typedef FsDirectory* FsDirectoryRPtr;
typedef FsDirectory const* FsDirectoryRCPtr;
typedef shared_ptr<FsDirectory> FsDirectorySPtr;
typedef Set<FsDirectory> FsDirectorySet;
typedef shared_ptr<FsDirectorySet> FsDirectorySetSPtr;

class FsDirectory : public StringTreeNode<FsDirectory, '/'>
{
public:
    static FsDirectorySPtr find(const FsDirectorySPtr& base,
                                const string_view& directory);

    static FsDirectorySPtr obtain(const FsDirectorySPtr& base,
                                  const string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    static FsDirectorySPtr corresponding(const FsDirectorySPtr& directory,
                                         const FsDirectorySPtr& fromDirectory,
                                         const FsDirectorySPtr& toDirectory);

    using StringTreeNode<FsDirectory, '/'>::StringTreeNode;

    auto parent() const
    {
        return ancestor();
    }

    using StringTreeNode<FsDirectory, '/'>::find;

    FsDirectorySPtr commonAncestor(const FsDirectorySPtr& directory) const;
    string path(const FsDirectorySPtr& directory = nullptr) const;

    // This is almost the same as path, but in case the current and the root directories
    // are the same instead of empty string it will return '.'
    string nonEmptyPath(const FsDirectorySPtr& directory) const;

private:
    void calculate(FsDirectoryRPtr directory, size_t length, string& path) const;
};
}
