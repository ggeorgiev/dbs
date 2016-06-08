//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_headers_mixin.hpp"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/protobufs_mixin.hpp"
#include "dom/element.hpp"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_directory.h"
#include "err/err.h"
#include <shared_ptr>
#include <str>

namespace dom
{
class CxxProgram;
typedef shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram : public Element<CxxProgram>,
                   public ProtobufsMixin<CxxProgram>,
                   public CxxFilesMixin<CxxProgram>,
                   public CxxHeadersMixin<CxxProgram>
{
public:
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
