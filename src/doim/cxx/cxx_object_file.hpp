//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

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
            return hashCF(file->cxxFile());
        }

        bool operator()(const CxxObjectFileSPtr& file1,
                        const CxxObjectFileSPtr& file2) const
        {
            return file1->cxxFile() == file2->cxxFile();
        }

        std::hash<CxxFileSPtr> hashCF;
    };

private:
    CxxFileSPtr mCxxFile;
    FsFileSPtr mFile;
};
}
