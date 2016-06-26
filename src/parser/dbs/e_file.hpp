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
    File(const doim::FsDirectorySPtr& location)
        : mDirectory(location)
    {
    }

    auto set()
    {
        return e_ref([this](I& i1, I& i2) {
            auto size = std::distance(i1, i2);
            mFile = doim::FsFile::obtain(mDirectory.mDirectory, string_view(&*i1, size));
        });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_path >> set();
    }

    doim::FsFileSPtr mFile;
    Directory mDirectory;
};

struct FileSet
{
    FileSet(const doim::FsDirectorySPtr& location)
        : mFile(location)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mFiles.clear(); });
    }

    auto insert()
    {
        return e_ref([this](I& i1, I& i2) { mFiles.insert(mFile.mFile); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_empty() >> reset() & *(mFile.rule(r_ws) >> insert());
    }

    doim::FsFileSet mFiles;
    File mFile;
};
}
