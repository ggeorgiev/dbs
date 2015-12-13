//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.hpp"

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
    CxxObjectFile(const CxxFileSPtr& cxxFile, const FsFileSPtr& file)
        : mCxxFile(cxxFile)
        , mFile(file)
    {
    }

    const CxxFileSPtr& cxxFile()
    {
        return mCxxFile;
    }

    const FsFileSPtr& file()
    {
        return mFile;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxObjectFileSPtr& file) const
        {
            return std::hash<CxxFileSPtr>()(file->cxxFile());
        }

        bool operator()(const CxxObjectFileSPtr& file1,
                        const CxxObjectFileSPtr& file2) const
        {
            return file1->cxxFile() == file2->cxxFile();
        }
    };

private:
    CxxFileSPtr mCxxFile;
    FsFileSPtr mFile;
};
}
