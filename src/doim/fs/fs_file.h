//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/tree/string_tree_leaf.hpp"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <boost/variant/variant.hpp>
#include <memory>
#include <str>
#include <string_view>
#include <tuple>

namespace doim
{
class FsFile;
typedef shared_ptr<FsFile> FsFileSPtr;
typedef Set<FsFile> FsFileSet;
typedef shared_ptr<FsFileSet> FsFileSetSPtr;

class FsFile : public StringTreeLeaf<FsFile, FsDirectory>
{
public:
    using StringTreeLeaf<FsFile, FsDirectory>::find;
    static FsFileSPtr find(const FsDirectorySPtr& base, const string_view& file);

    static FsFileSPtr obtain(const FsDirectorySPtr& base, const string_view& file);

    using StringTreeLeaf<FsFile, FsDirectory>::StringTreeLeaf;

    auto directory() const
    {
        return ancestor();
    }

    string path(const FsDirectorySPtr& root = nullptr) const
    {
        return directory()->path(root) + name();
    }
};

namespace vst
{
struct path : public boost::static_visitor<string>
{
    path(const FsDirectorySPtr& directory = nullptr)
        : mDirectory(directory)
    {
    }

    template <typename T>
    string operator()(const T& element) const
    {
        return element->file()->path(mDirectory);
    }

private:
    FsDirectorySPtr mDirectory;
};
}
}
