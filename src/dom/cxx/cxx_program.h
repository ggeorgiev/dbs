//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_library.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
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

    std::string name() const
    {
        return mName;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries);

    ECode updateName(const std::string& name);

    // Computations
    doim::CxxProgramSPtr cxxProgram(const doim::FsDirectorySPtr& root,
                                    const doim::FsDirectorySPtr& intermediate) const;

    std::unordered_set<CxxLibrarySPtr> recursiveCxxLibraries() const;

    doim::CxxIncludeDirectorySetSPtr recursiveCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    doim::CxxHeaderSetSPtr cxxHeaders(const doim::FsDirectorySPtr& root) const;

private:
    std::string mName;
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
