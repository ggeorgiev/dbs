//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered>
#include <stddef.h>

namespace doim
{
class FsDirectory;

typedef FsDirectory* FsDirectoryRPtr;
typedef FsDirectory const* FsDirectoryRCPtr;
typedef std::shared_ptr<FsDirectory> FsDirectorySPtr;
typedef unordered_set<FsDirectorySPtr> FsDirectorySet;
typedef std::shared_ptr<FsDirectorySet> FsDirectorySetSPtr;

class FsDirectory : public Base<FsDirectory, FsDirectorySPtr, std::string>
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
                mFsDirectory = FsDirectory::make();
        }

        void reset()
        {
            mFsDirectory.reset();
        }

        void set_parent(const FsDirectorySPtr& parent) const
        {
            std::get<0>(mFsDirectory->mArgs) = parent;
            mFsDirectory->mLevel = parent == nullptr ? 1 : parent->level() + 1;
        }

        void set_name(const std::string& name) const
        {
            std::get<1>(mFsDirectory->mArgs) = name;
        }

    private:
        FsDirectorySPtr mFsDirectory;
    };

    static FsDirectorySPtr global(const FsDirectorySPtr& parent,
                                  const std::string& name,
                                  FsDirectorySPtr& directory);

    FsDirectory();
    FsDirectory(const FsDirectorySPtr& parent, const std::string& name);

    const FsDirectorySPtr& parent() const
    {
        return std::get<0>(mArgs);
    }

    const std::string& name() const
    {
        return std::get<1>(mArgs);
    }

    size_t level() const
    {
        return mLevel;
    }

    FsDirectorySPtr commonAncestor(const FsDirectorySPtr& directory) const;
    std::string path(const FsDirectorySPtr& directory = nullptr) const;

private:
    void calculate(FsDirectoryRPtr directory, size_t length, std::string& path) const;

    size_t mLevel;
};
}
