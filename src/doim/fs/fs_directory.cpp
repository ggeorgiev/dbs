//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"
#include "err/err_assert.h"
#include "const/constants.h"
#include <algorithm>

namespace doim
{
template <typename Trace>
FsDirectorySPtr traceDirectory(Trace&& trace,
                               const FsDirectorySPtr& base,
                               const string_view& directory)
{
    if (directory.empty())
        return base;

    auto parent = directory.front() == slash() ? nullptr : base;

    ASSERT(parent != nullptr || directory.front() == slash());
    ASSERT(parent == nullptr || directory.front() != slash());

    auto pos = directory.begin();
    while (pos < directory.end())
    {
        const auto& next = std::find(pos, directory.end(), slash());

        if (next != pos || parent == nullptr)
        {
            string name(pos, next);
            if (name == kParentDirectoryString)
            {
                if (parent == nullptr)
                    return nullptr;
                parent = parent->parent();
            }
            else if (name != kCurrentDirectoryString)
            {
                const auto& current = FsDirectory::make(parent, name);
                parent = trace(current);

                if (parent == nullptr)
                    return parent;
            }
            if (next == directory.end())
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = FsDirectory::make(nullptr, string());

    return trace(parent);
}

FsDirectorySPtr FsDirectory::find(const FsDirectorySPtr& base,
                                  const string_view& directory)
{
    auto trace = [](const FsDirectorySPtr& directory) -> FsDirectorySPtr {
        return directory->find();
    };

    return traceDirectory(trace, base, directory);
}

FsDirectorySPtr FsDirectory::obtain(const FsDirectorySPtr& base,
                                    const string_view& directory)
{
    auto trace = [](const FsDirectorySPtr& directory) -> FsDirectorySPtr {
        return FsDirectory::unique(directory);
    };

    return traceDirectory(trace, base, directory);
}

FsDirectorySPtr FsDirectory::corresponding(const FsDirectorySPtr& directory,
                                           const FsDirectorySPtr& fromDirectory,
                                           const FsDirectorySPtr& toDirectory)
{
    ASSERT(directory != nullptr);

    if (directory == fromDirectory)
        return toDirectory;

    const auto& parent = corresponding(directory->parent(), fromDirectory, toDirectory);
    return obtain(parent, directory->name());
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

string FsDirectory::path(const FsDirectorySPtr& directory) const
{
    FsDirectorySPtr base = commonAncestor(directory);

    string path;
    if (base != nullptr)
    {
        if (base->parent() == nullptr && base != directory)
        {
            path = "/";
        }
        else
        {
            auto levels = (directory != nullptr ? directory->level() : 0) - base->level();
            if (levels > 0)
            {
                path.reserve(levels * 3);
                for (auto i = 0; i < levels; ++i)
                    path += "../";
            }
        }
    }

    if (base.get() != this)
        calculate(base.get(), path.size(), path);

    return path;
}

string FsDirectory::nonEmptyPath(const FsDirectorySPtr& directory) const
{
    if (directory.get() == this)
        return ".";
    return path(directory);
}

void FsDirectory::calculate(FsDirectoryRPtr directory, size_t length, string& path) const
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
