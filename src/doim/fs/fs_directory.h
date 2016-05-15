//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>
#include <stddef.h>

namespace doim
{
class FsDirectory;

typedef FsDirectory* FsDirectoryRPtr;
typedef FsDirectory const* FsDirectoryRCPtr;
typedef shared_ptr<FsDirectory> FsDirectorySPtr;
typedef NewObjectSet<FsDirectory> FsDirectorySet;
typedef shared_ptr<FsDirectorySet> FsDirectorySetSPtr;

class FsDirectory : public Base<FsDirectory, FsDirectorySPtr, string>
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

        void set_name(const string& name) const
        {
            std::get<1>(mFsDirectory->mArgs) = name;
        }

    private:
        FsDirectorySPtr mFsDirectory;
    };

    using Base<FsDirectory, FsDirectorySPtr, string>::find;
    static FsDirectorySPtr find(const FsDirectorySPtr& base,
                                const string_view& directory);

    static FsDirectorySPtr obtain(const FsDirectorySPtr& base,
                                  const string_view& directory);

    // Obtain the unique corresponding directory of directory in toDirectory to that in
    // fromDirectory. Note that directory must be subdirectory of fromDirectory.
    static FsDirectorySPtr corresponding(const FsDirectorySPtr& directory,
                                         const FsDirectorySPtr& fromDirectory,
                                         const FsDirectorySPtr& toDirectory);

    FsDirectory();
    FsDirectory(const FsDirectorySPtr& parent, const string& name);

    const FsDirectorySPtr& parent() const
    {
        return std::get<0>(mArgs);
    }

    const string& name() const
    {
        return std::get<1>(mArgs);
    }

    size_t level() const
    {
        return mLevel;
    }

    FsDirectorySPtr commonAncestor(const FsDirectorySPtr& directory) const;
    string path(const FsDirectorySPtr& directory = nullptr) const;

private:
    void calculate(FsDirectoryRPtr directory, size_t length, string& path) const;

    size_t mLevel;
};
}
