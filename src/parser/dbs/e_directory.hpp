//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_position.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "err/err.h"

namespace parser
{
static auto r_slash = r_char('/');
static auto r_illegal = r_any("/?<>\\:;*|\"");
static auto r_pathChars = r_any() - r_illegal - r_space - r_endl;
static auto r_path = ~r_slash & +r_pathChars & *(r_slash & +r_pathChars) & ~r_slash;

struct Directory
{
    Directory(const doim::FsDirectorySPtr& directory)
        : mDefaultDirectory(directory)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mDirectory = mDefaultDirectory; });
    }

    doim::FsDirectorySPtr mDirectory;
    doim::FsDirectorySPtr mDefaultDirectory;
};
}
