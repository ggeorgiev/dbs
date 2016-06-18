//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_file_set.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "dom/cxx/cxx_program.h"

namespace parser
{
static auto r_cxxProgramKw = r_str("cxx_program");

struct CxxProgram
{
    CxxProgram(Object& object, FileSet& files, CxxLibrarySet& cxxLibraries)
        : mObject(object)
        , mFiles(files)
        , mCxxLibraries(cxxLibraries)
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
            mCxxProgram = dom::CxxProgram::obtain(mObject.mObject);
            mCxxProgram->updateName(mObject.mObject->name());
        });
    }

    Object& mObject;
    FileSet& mFiles;
    CxxLibrarySet& mCxxLibraries;

    dom::CxxProgramSPtr mCxxProgram;
};
}
