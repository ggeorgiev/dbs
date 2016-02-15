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
            const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
        : mFile(file)
        , mCxxIncludeDirectories(cxxIncludeDirectories)
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

    struct Hasher
    {
        std::size_t operator()(const CxxFileSPtr& cxxFile) const
        {
            auto seed = hashFF(cxxFile->file());
            boost::hash_combine(seed, hashCIDS(cxxFile->cxxIncludeDirectories()));
            return seed;
        }

        bool operator()(const CxxFileSPtr& cxxFile1, const CxxFileSPtr& cxxFile2) const
        {
            return cxxFile1->file() == cxxFile2->file() &&
                   cxxFile1->cxxIncludeDirectories() == cxxFile2->cxxIncludeDirectories();
        }

        std::hash<FsFileSPtr> hashFF;
        std::hash<CxxIncludeDirectorySetSPtr> hashCIDS;
    };

private:
    FsFileSPtr mFile;
    CxxIncludeDirectorySetSPtr mCxxIncludeDirectories;
};

typedef std::unordered_set<CxxFileSPtr> CxxFileSet;
typedef std::shared_ptr<CxxFileSet> CxxFileSetSPtr;

struct CxxFileSetHasher
{
    std::size_t operator()(const CxxFileSetSPtr& files) const
    {
        std::vector<CxxFileSPtr> vector(files->size());
        vector.insert(vector.begin(), files->begin(), files->end());
        std::sort(vector.begin(), vector.end());

        std::size_t seed = 0;
        for (const auto& file : vector)
            boost::hash_combine(seed, hashCF(file));

        return seed;
    }

    bool operator()(const CxxFileSetSPtr& files1, const CxxFileSetSPtr& files2) const
    {
        return *files1 == *files2;
    }

    std::hash<CxxFileSPtr> hashCF;
};
}
