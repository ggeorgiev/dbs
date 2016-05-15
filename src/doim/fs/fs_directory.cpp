//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_directory.h"

#include "err/err_assert.h"
#include "const/constants.h"
#include "functional"

namespace doim
{
FsDirectory::FsDirectory()
    : Element(FsDirectorySPtr(), string())
    , mLevel(1)
{
}

FsDirectory::FsDirectory(const FsDirectorySPtr& parent, const string& name)
    : Element(parent, name)
    , mLevel(parent == nullptr ? 1 : parent->level() + 1)
{
    ASSERT(parent->isUnique());
}

template <typename Trace>
FsDirectorySPtr traceDirectory(Trace&& trace,
                               const FsDirectorySPtr& base,
                               const string_view& directory)
{
    if (directory.empty())
        return base;

    FsDirectory::Builder builder;
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
                builder.ensure();
                builder.set_parent(parent);
                builder.set_name(name);

                const auto& current = builder.reference();
                parent = trace(current);

                if (parent == nullptr)
                    return parent;

                if (parent == current)
                    builder.reset();
            }
            if (next == directory.end())
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = FsDirectory::make();

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

    auto levels = (directory != nullptr ? directory->level() : 0) -
                  (base != nullptr ? base->level() : 0);

    string path;

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
