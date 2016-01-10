//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_library.hpp"
#include "doim/fs/fs_file.hpp"
#include "err/err.h"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
class CxxProgram;
typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram : public CxxFilesMixin<CxxProgram>,
                   public CxxPrivateHeadersMixin<CxxProgram>
{
public:
    template <typename T>
    std::vector<T> difference(const std::unordered_set<T>& a,
                              const std::unordered_set<T>& b)
    {
        std::vector<T> result;
        std::copy_if(a.begin(), a.end(), back_inserter(result), [&b](T needle) {
            return b.find(needle) == b.end();
        });
        return result;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
        EHEnd;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries()
    {
        return mCxxLibraries;
    }

    // Computations
    doim::CxxIncludeDirectorySetSPtr cxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->cxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

    doim::CxxHeaderSetSPtr cxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libHeaders = cxxLibrary->cxxHeaders(root);
            headers->insert(libHeaders->begin(), libHeaders->end());
        }
        return doim::gManager->unique(headers);
    }

private:
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
