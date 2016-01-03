//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "const/constants.h"
#include <iostream>
#include <memory>
#include <string>

namespace doim
{
class FsDirectory;
typedef std::shared_ptr<FsDirectory> FsDirectorySPtr;

class FsDirectory
{
public:
    class Builder
    {
    public:
        const FsDirectorySPtr& reference()
        {
            return mFsDirectory;
        }

        void ensure()
        {
            if (mFsDirectory == nullptr)
                mFsDirectory = std::make_shared<FsDirectory>();
        }

        void reset()
        {
            mFsDirectory.reset();
        }

        void set_parent(const FsDirectorySPtr& parent) const
        {
            mFsDirectory->mParent = parent;
        }

        void set_name(const std::string& name) const
        {
            mFsDirectory->mName = name;
        }

    private:
        FsDirectorySPtr mFsDirectory;
    };

    FsDirectory()
    {
    }

    FsDirectory(const FsDirectorySPtr& parent, const std::string& name)
        : mParent(parent)
        , mName(name)
    {
    }

    std::string path(const FsDirectorySPtr& directory = nullptr)
    {
        size_t dlevel = (directory == nullptr) ? 0 : directory->level();
        size_t clevel = level();

        FsDirectorySPtr base = directory;

        std::string path;
        while (dlevel > clevel)
        {
            path += "../";
            base = base->parent();
            --dlevel;
        }

        if (this == base.get())
            return path;

        calculate(directory, path.size(), path);
        return path;
    }

    const std::string& name() const
    {
        return mName;
    }

    const FsDirectorySPtr& parent() const
    {
        return mParent;
    }

    size_t level()
    {
        size_t level = 1;
        auto current = parent();
        while (current != nullptr)
        {
            ++level;
            current = current->parent();
        }
        return level;
    }

    struct Hasher
    {
        std::size_t operator()(const FsDirectorySPtr& directory) const
        {
            return std::hash<FsDirectorySPtr>()(directory->parent()) ^
                   std::hash<std::string>()(directory->name());
        }

        bool operator()(const FsDirectorySPtr& directory1,
                        const FsDirectorySPtr& directory2) const
        {
            return directory1->parent() == directory2->parent() &&
                   directory1->name() == directory2->name();
        }
    };

private:
    void calculate(const FsDirectorySPtr& directory, size_t length, std::string& path)
    {
        length += name().size() + 1;

        if (parent() == directory)
            path.reserve(length);
        else
            parent()->calculate(directory, length, path);

        path += name();
        path += slash();
    }

    std::string mName;
    FsDirectorySPtr mParent;
};
}