//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "dom/cxx/cxx_program.h"

namespace parser
{
static auto r_cxxProgramKw = r_str("cxx_program");

typedef ObjectRef<dom::CxxProgram> CxxProgramRef;

struct CxxProgram
{
    CxxProgram(const doim::FsDirectorySPtr& location)
        : mCxxProgramRef(location)
        , mFiles(location)
        , mCxxLibraries(location)
    {
    }

    auto libraries()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxProgram->updateCxxLibraries(mCxxLibraries.mCxxLibraries);
        });
    }

    auto files()
    {
        return e_ref(
            [this](I& i1, I& i2) { mCxxProgram->updateCxxFilesList(mFiles.mFiles); });
    }

    auto name()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxProgram = mCxxProgramRef.mObjectRef;
            mCxxProgram->updateName(mCxxProgramRef.mObject.mObject->name());
        });
    }

    template <typename WS>
    auto r_cxxFiles(const WS& r_ws)
    {
        return (r_ws & r_cxxFileKw & r_ws & r_colon & mFiles.mFile.mDirectory.r_reset() &
                mFiles.rule(r_ws) & r_ws & r_semicolon) >>
               files();
    }

    template <typename WS>
    auto r_cxxLibrary(const WS& r_ws)
    {
        return mCxxLibraries.rule(r_ws) >> libraries();
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_cxxProgramKw & mCxxProgramRef.rule(r_ws) >> name() & r_ws &
               r_colon & *(r_cxxFiles(r_ws) | r_cxxLibrary(r_ws)) & r_ws & r_semicolon;
    }

    CxxProgramRef mCxxProgramRef;
    FileSet mFiles;
    CxxLibrarySet mCxxLibraries;

    dom::CxxProgramSPtr mCxxProgram;
};
}
