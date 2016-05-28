//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <tuple>

namespace doim
{
class CxxObjectFile;
typedef shared_ptr<CxxObjectFile> CxxObjectFileSPtr;
typedef Set<CxxObjectFile> CxxObjectFileSet;
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
      public Element<CxxObjectFile, CxxObjectFileEnums::EPurpose, CxxFileSPtr, FsFileSPtr>
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
