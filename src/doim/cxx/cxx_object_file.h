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
typedef std::shared_ptr<CxxObjectFile> CxxObjectFileSPtr;
typedef unordered_set<CxxObjectFileSPtr> CxxObjectFileSet;
typedef std::shared_ptr<CxxObjectFileSet> CxxObjectFileSetSPtr;

namespace cof_details
{
enum class EPurpose
{
    kDebug,
    kRelease,
    kProfile,
};
}

class CxxObjectFile : public Base<CxxObjectFile,
                                  std::underlying_type<cof_details::EPurpose>::type,
                                  CxxFileSPtr,
                                  FsFileSPtr>
{
public:
    typedef cof_details::EPurpose EPurpose;

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
