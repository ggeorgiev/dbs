//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.h"
#include "doim/element.hpp"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>

namespace doim
{
class CxxHeader;
typedef shared_ptr<CxxHeader> CxxHeaderSPtr;
typedef Set<CxxHeader> CxxHeaderSet;
typedef shared_ptr<CxxHeaderSet> CxxHeaderSetSPtr;

struct CxxHeaderEnums
{
    enum class EType
    {
        kUser,
        kSystem,
    };
};

class CxxHeader : public CxxHeaderEnums,
                  public Element<CxxHeader,
                                 CxxHeaderEnums::EType,
                                 FsFileSPtr,
                                 CxxIncludeDirectorySetSPtr>
{
public:
    CxxHeader(const EType type,
              const FsFileSPtr& file,
              const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories);

    EType type() const
    {
        return std::get<0>(mArgs);
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

std::ostream& operator<<(std::ostream& out, const CxxHeader& header);
}
