//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxObjectFile;

typedef std::shared_ptr<CxxObjectFile> CxxObjectFileSPtr;
typedef std::unordered_set<CxxObjectFileSPtr> CxxObjectFileSet;
typedef std::shared_ptr<CxxObjectFileSet> CxxObjectFileSetSPtr;

class CxxObjectFile : public Base<CxxObjectFile, CxxFileSPtr, FsFileSPtr>
{
public:
    CxxObjectFile(const CxxFileSPtr& cxxFile, const FsFileSPtr& file);

    const CxxFileSPtr& cxxFile()
    {
        return std::get<0>(mArgs);
    }

    const FsFileSPtr& file()
    {
        return std::get<1>(mArgs);
    }
};
}
