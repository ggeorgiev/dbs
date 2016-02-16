//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace doim
{
class CxxHeader;

typedef std::shared_ptr<CxxHeader> CxxHeaderSPtr;
typedef std::unordered_set<CxxHeaderSPtr> CxxHeaderSet;
typedef std::shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

class CxxHeader : public Base<CxxHeader, int, FsFileSPtr, CxxIncludeDirectorySetSPtr>
{
public:
    enum class Type
    {
        kUser,
        kSystem,
    };

    CxxHeader(const Type type,
              const FsFileSPtr& file,
              const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories);

    Type type() const
    {
        return static_cast<Type>(std::get<0>(mArgs));
    }

    const FsFileSPtr& file() const
    {
        return std::get<1>(mArgs);
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return std::get<2>(mArgs);
    }
};
}
