//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include "const/constants.h"
#include "functional"

namespace doim
{
FsDirectorySPtr FsDirectory::global(const FsDirectorySPtr& parent,
                                    const std::string& name,
                                    FsDirectorySPtr& directory)
{
    return Manager::global<FsDirectory, FsDirectorySPtr, std::string>(parent,
                                                                      name,
                                                                      directory);
}

FsDirectory::FsDirectory()
    : Base(FsDirectorySPtr(), std::string())
    , mLevel(1)
{
}

FsDirectory::FsDirectory(const FsDirectorySPtr& parent, const std::string& name)
    : Base(parent, name)
    , mLevel(parent == nullptr ? 1 : parent->level() + 1)
{
    ASSERT(gManager->isUnique(parent));
}

FsDirectorySPtr FsDirectory::commonAncestor(const FsDirectorySPtr& directory) const
{
    if (directory == nullptr)
        return nullptr;

    FsDirectorySPtr line1 = directory;

    while (line1->level() > level())
        line1 = line1->parent();

    if (line1.get() == this)
        return line1;

    FsDirectoryRCPtr line2 = this;
    while (line1->level() < line2->level())
        line2 = line2->parent().get();

    while (line1.get() != line2)
    {
        line1 = line1->parent();
        line2 = line2->parent().get();
    }

    return line1;
}

std::string FsDirectory::path(const FsDirectorySPtr& directory) const
{
    FsDirectorySPtr base = commonAncestor(directory);

    auto levels = (directory != nullptr ? directory->level() : 0) -
                  (base != nullptr ? base->level() : 0);

    std::string path;

    if (levels > 0)
    {
        path.reserve(levels * 3);
        for (auto i = 0; i < levels; ++i)
            path += "../";
    }

    if (base.get() != this)
        calculate(base.get(), path.size(), path);

    return path;
}

void FsDirectory::calculate(FsDirectoryRPtr directory,
                            size_t length,
                            std::string& path) const
{
    length += name().size() + 1;

    if (parent().get() == directory)
        path.reserve(length);
    else
        parent()->calculate(directory, length, path);

    path += name();
    path += slash();
}
}
