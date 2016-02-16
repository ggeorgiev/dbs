//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.hpp"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxProgram;

typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;
typedef std::unordered_set<CxxProgramSPtr> CxxProgramSet;
typedef std::shared_ptr<CxxProgramSet> CxxProgramSetSPtr;

class CxxProgram : public Base<CxxProgram, FsFileSPtr, CxxObjectFileSetSPtr>
{
public:
    CxxProgram(const FsFileSPtr& file, const CxxObjectFileSetSPtr& cxxObjectFiles);

    const FsFileSPtr& file() const
    {
        return std::get<0>(mArgs);
    }

    const CxxObjectFileSetSPtr& cxxObjectFiles()
    {
        return std::get<1>(mArgs);
    }
};
}
