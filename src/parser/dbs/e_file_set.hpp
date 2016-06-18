//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_file.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "err/err.h"

namespace parser
{
struct FileSet
{
    FileSet(File& file)
        : mFile(file)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mFiles.clear(); });
    }

    void operator()(I& i1, I& i2)
    {
        mFiles.insert(mFile.mFile);
    };

    doim::FsFileSet mFiles;
    File& mFile;
};
}
