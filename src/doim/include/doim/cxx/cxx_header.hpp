//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.hpp"
#include "doim/fs/fs_file.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class CxxHeader;
typedef std::shared_ptr<CxxHeader> CxxHeaderSPtr;

typedef std::unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef std::shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxHeader
{
public:
    CxxHeader(const FsFileSPtr& file,
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
        std::size_t operator()(const CxxHeaderSPtr& cxxFile) const
        {
            return std::hash<FsFileSPtr>()(cxxFile->file()) ^
                   std::hash<CxxIncludeDirectorySetSPtr>()(
                       cxxFile->cxxIncludeDirectories()) ^
                   std::hash<CxxHeaderSetSPtr>()(cxxFile->cxxHeaders());
        }

        bool operator()(const CxxHeaderSPtr& cxxFile1,
                        const CxxHeaderSPtr& cxxFile2) const
        {
            return cxxFile1->file() == cxxFile2->file() &&
                   cxxFile1->cxxIncludeDirectories() ==
                       cxxFile2->cxxIncludeDirectories() &&
                   cxxFile1->cxxHeaders() == cxxFile2->cxxHeaders();
        }
    };

private:
    FsFileSPtr mFile;
    CxxIncludeDirectorySetSPtr mCxxIncludeDirectories;
    CxxHeaderSetSPtr mCxxHeaders;
};

struct CxxHeaderSetHasher
{
    std::size_t operator()(const CxxHeaderSetSPtr& headers) const
    {
        std::vector<CxxHeaderSPtr> vector(headers->size());
        vector.insert(vector.begin(), headers->begin(), headers->end());
        std::sort(vector.begin(), vector.end());

        std::size_t hash = 0;
        for (const auto& header : vector)
            hash ^ std::hash<CxxHeaderSPtr>()(header);

        return hash;
    }

    bool operator()(const CxxHeaderSetSPtr& headers1,
                    const CxxHeaderSetSPtr& headers2) const
    {
        return *headers1 == *headers2;
    }
};
}
