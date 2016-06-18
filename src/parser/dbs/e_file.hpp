//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "err/err.h"

namespace parser
{
static auto r_binaryKw = r_str("binary");

struct File
{
    File(Directory& directory)
        : mDirectory(directory)
    {
    }

    void operator()(I& i1, I& i2)
    {
        auto size = std::distance(i1, i2);
        mFile = doim::FsFile::obtain(mDirectory.mDirectory, string_view(&*i1, size));
    };

    doim::FsFileSPtr mFile;
    Directory& mDirectory;
};
}
