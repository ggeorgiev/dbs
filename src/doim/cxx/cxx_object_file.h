//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered>

namespace doim
{
class CxxObjectFile;
typedef shared_ptr<CxxObjectFile> CxxObjectFileSPtr;
typedef unordered_set<CxxObjectFileSPtr> CxxObjectFileSet;
typedef shared_ptr<CxxObjectFileSet> CxxObjectFileSetSPtr;

struct CxxObjectFileEnums
{
    enum class EPurpose
    {
        kDebug,
        kRelease,
        kProfile,
    };
};

class CxxObjectFile
    : public CxxObjectFileEnums,
      public Base<CxxObjectFile, CxxObjectFileEnums::EPurpose, CxxFileSPtr, FsFileSPtr>
{
public:
    CxxObjectFile(EPurpose purpose, const CxxFileSPtr& cxxFile, const FsFileSPtr& file);

    EPurpose purpose() const
    {
        return static_cast<EPurpose>(std::get<0>(mArgs));
    }

    const CxxFileSPtr& cxxFile()
    {
        return std::get<1>(mArgs);
    }

    const FsFileSPtr& file()
    {
        return std::get<2>(mArgs);
    }
};
}
