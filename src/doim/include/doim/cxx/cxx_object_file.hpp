//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.hpp"

#include <memory>
#include <string>
#include <utility>

namespace doim
{
class CxxObjectFile
{
public:
    CxxObjectFile(const FsFileSPtr& cxxFile,
                  const std::vector<FsDirectorySPtr>& cxxIncludeDirectories,
                  const FsFileSPtr& outputFile)
        : mCxxFile(cxxFile)
        , mCxxIncludeDirectories(cxxIncludeDirectories)
        , mOutputFile(outputFile)
    {
    }

private:
    FsFileSPtr mCxxFile;
    std::vector<FsDirectorySPtr> mCxxIncludeDirectories;
    FsFileSPtr mOutputFile;
};

typedef std::shared_ptr<FsFile> FsFileSPtr;
}
