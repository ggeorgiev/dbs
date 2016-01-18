//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "const/constants.h"
#include <boost/functional/hash.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace doim
{
class FsDirectory;
typedef FsDirectory* FsDirectoryRPtr;
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
            mFsDirectory->mLevel = parent == nullptr ? 1 : parent->level() + 1;
        }

        void set_name(const std::string& name) const
        {
            mFsDirectory->mName = name;
        }

    private:
        FsDirectorySPtr mFsDirectory;
    };

    FsDirectory()
        : mLevel(1)
    {
    }

    FsDirectory(const FsDirectorySPtr& parent, const std::string& name)
        : mParent(parent)
        , mName(name)
        , mLevel(parent == nullptr ? 1 : parent->level() + 1)
    {
    }

    FsDirectorySPtr commonAncestor(const FsDirectorySPtr& directory)
    {
        if (directory == nullptr)
            return nullptr;

        FsDirectorySPtr line1 = directory;

        while (line1->level() > level())
            line1 = line1->parent();

        if (line1.get() == this)
            return line1;

        FsDirectoryRPtr line2 = this;
        while (line1->level() < line2->level())
            line2 = line2->parent().get();

        while (line1.get() != line2)
        {
            line1 = line1->parent();
            line2 = line2->parent().get();
        }

        return line1;
    }

    std::string path(const FsDirectorySPtr& directory = nullptr)
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
        return mLevel;
    }

    struct Hasher
    {
        std::size_t operator()(const FsDirectorySPtr& directory) const
        {
            auto seed = hashFD(directory->parent());
            boost::hash_combine(seed, hashString(directory->name()));
            return seed;
        }

        bool operator()(const FsDirectorySPtr& directory1,
                        const FsDirectorySPtr& directory2) const
        {
            return directory1->parent() == directory2->parent() &&
                   directory1->name() == directory2->name();
        }

        std::hash<FsDirectorySPtr> hashFD;
        std::hash<std::string> hashString;
    };

private:
    void calculate(FsDirectoryRPtr directory, size_t length, std::string& path)
    {
        length += name().size() + 1;

        if (parent().get() == directory)
            path.reserve(length);
        else
            parent()->calculate(directory, length, path);

        path += name();
        path += slash();
    }

    std::string mName;
    FsDirectorySPtr mParent;
    size_t mLevel;
};
}
