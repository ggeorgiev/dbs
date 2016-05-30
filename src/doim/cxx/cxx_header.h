//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <boost/variant/variant.hpp>
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

    enum class EVisibility
    {
        // Header file that is visible to the library and all libraries that depend on it.
        kPublic,

        // Header that is visible to the library files and public headers, but not to the
        // dependent libraries.
        kProtected,

        // Header that is visible only to the library files.
        kPrivate,
    };
};

struct CxxHeaderVariants
{
    typedef boost::variant<ProtobufFileSPtr> OriginSPtr;
};

class CxxHeader : public CxxHeaderEnums,
                  public CxxHeaderVariants,
                  public Element<CxxHeader,
                                 CxxHeaderEnums::EType,
                                 CxxHeaderEnums::EVisibility,
                                 FsFileSPtr,
                                 CxxIncludeDirectorySetSPtr,
                                 CxxHeaderVariants::OriginSPtr>
{
public:
    CxxHeader(const EType type,
              const EVisibility visibility,
              const FsFileSPtr& file,
              const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories,
              const OriginSPtr& origin);

    EType type() const
    {
        return std::get<0>(mArgs);
    }

    EVisibility visibility() const
    {
        return std::get<1>(mArgs);
    }

    const FsFileSPtr& file() const
    {
        return std::get<2>(mArgs);
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return std::get<3>(mArgs);
    }

    const OriginSPtr& origin() const
    {
        return std::get<4>(mArgs);
    }
};

std::ostream& operator<<(std::ostream& out, const CxxHeader& header);
}
