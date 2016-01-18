//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

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
class CxxHeader;
typedef std::shared_ptr<CxxHeader> CxxHeaderSPtr;

typedef std::unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef std::shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxHeader
{
public:
    enum class Type
    {
        kUser,
        kSystem,
    };

    CxxHeader(const Type type,
              const FsFileSPtr& file,
              const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
        : mType(type)
        , mFile(file)
        , mCxxIncludeDirectories(cxxIncludeDirectories)
    {
    }

    Type type()
    {
        return mType;
    }

    const FsFileSPtr& file() const
    {
        return mFile;
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return mCxxIncludeDirectories;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxHeaderSPtr& headerFile) const
        {
            auto seed = std::hash<int>()(static_cast<int>(headerFile->type()));
            boost::hash_combine(seed, hashFF(headerFile->file()));
            boost::hash_combine(seed, hashCIDS(headerFile->cxxIncludeDirectories()));
            return seed;
        }

        bool operator()(const CxxHeaderSPtr& headerFile1,
                        const CxxHeaderSPtr& headerFile2) const
        {
            return headerFile1->type() == headerFile2->type() &&
                   headerFile1->file() == headerFile2->file() &&
                   headerFile1->cxxIncludeDirectories() ==
                       headerFile2->cxxIncludeDirectories();
        }

        std::hash<FsFileSPtr> hashFF;
        std::hash<CxxIncludeDirectorySetSPtr> hashCIDS;
    };

private:
    Type mType;
    FsFileSPtr mFile;
    CxxIncludeDirectorySetSPtr mCxxIncludeDirectories;
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
