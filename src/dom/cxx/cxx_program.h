//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_headers_mixin.hpp"
#include "dom/cxx/cxx_library.h"
#include "dom/element.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_directory.h"
#include "err/err.h"
#include <algorithm>
#include <shared_ptr>
#include <str>
#include <unordered>
#include <vector>

namespace dom
{
class CxxProgram;
typedef shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram : public Element<CxxProgram>,
                   public CxxFilesMixin<CxxProgram>,
                   public CxxHeadersMixin<CxxProgram>
{
public:
    template <typename T>
    std::vector<T> difference(const unordered_set<T>& a, const unordered_set<T>& b)
    {
        std::vector<T> result;
        std::copy_if(a.begin(), a.end(), back_inserter(result), [&b](T needle) {
            return b.find(needle) == b.end();
        });
        return result;
    }

    string name() const
    {
        return mName;
    }

    const CxxLibrarySet& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    ECode updateCxxLibraries(CxxLibrarySet& libraries);

    ECode updateName(const string& name);

    // Computations
    doim::CxxProgramSPtr cxxProgram(doim::CxxProgram::EPurpose purpose,
                                    const doim::FsDirectorySPtr& root,
                                    const doim::FsDirectorySPtr& intermediate) const;

    CxxLibrarySet recursiveCxxLibraries() const;

    doim::CxxIncludeDirectorySetSPtr visibleCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

private:
    string mName;
    CxxLibrarySet mCxxLibraries;
};
}
