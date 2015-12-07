//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/fs/fs_file.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

namespace doim
{
class CxxObjectFile;
typedef std::shared_ptr<CxxObjectFile> CxxObjectFileSPtr;

class CxxObjectFile
{
public:
    CxxObjectFile(
        const FsFileSPtr& cxxFile,
        const std::unordered_set<CxxIncludeDirectorySPtr>& cxxIncludeDirectories,
        const FsFileSPtr& outputFile)
        : mCxxFile(cxxFile)
        , mCxxIncludeDirectories(cxxIncludeDirectories)
        , mOutputFile(outputFile)
    {
    }

    const FsFileSPtr& cxxFile()
    {
        return mCxxFile;
    }

    const FsFileSPtr& outputFile()
    {
        return mOutputFile;
    }

    const std::unordered_set<CxxIncludeDirectorySPtr>& cxxIncludeDirectories()
    {
        return mCxxIncludeDirectories;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxObjectFileSPtr& file) const
        {
            return std::hash<FsFileSPtr>()(file->cxxFile());
        }

        bool operator()(const CxxObjectFileSPtr& file1,
                        const CxxObjectFileSPtr& file2) const
        {
            return file1->cxxFile() == file2->cxxFile();
        }
    };

private:
    FsFileSPtr mCxxFile;
    std::unordered_set<CxxIncludeDirectorySPtr> mCxxIncludeDirectories;
    FsFileSPtr mOutputFile;
};
}
