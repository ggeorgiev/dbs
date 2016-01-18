//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_header.hpp"
#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class CxxFile;
typedef std::shared_ptr<CxxFile> CxxFileSPtr;

class CxxFile
{
public:
    CxxFile(const FsFileSPtr& file,
            const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories,
            const CxxHeaderSetSPtr& cxxHeaders)
        : mFile(file)
        , mCxxIncludeDirectories(cxxIncludeDirectories)
        , mCxxHeaders(cxxHeaders)
    {
    }

    const FsFileSPtr& file() const
    {
        return mFile;
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return mCxxIncludeDirectories;
    }

    const CxxHeaderSetSPtr& cxxHeaders() const
    {
        return mCxxHeaders;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxFileSPtr& cxxFile) const
        {
            auto seed = hashFF(cxxFile->file());
            boost::hash_combine(seed, hashCIDS(cxxFile->cxxIncludeDirectories()));
            boost::hash_combine(seed, hashCHS(cxxFile->cxxHeaders()));
            return seed;
        }

        bool operator()(const CxxFileSPtr& cxxFile1, const CxxFileSPtr& cxxFile2) const
        {
            return cxxFile1->file() == cxxFile2->file() &&
                   cxxFile1->cxxIncludeDirectories() ==
                       cxxFile2->cxxIncludeDirectories() &&
                   cxxFile1->cxxHeaders() == cxxFile2->cxxHeaders();
        }

        std::hash<FsFileSPtr> hashFF;
        std::hash<CxxIncludeDirectorySetSPtr> hashCIDS;
        std::hash<CxxHeaderSetSPtr> hashCHS;
    };

private:
    FsFileSPtr mFile;
    CxxIncludeDirectorySetSPtr mCxxIncludeDirectories;
    CxxHeaderSetSPtr mCxxHeaders;
};
}
