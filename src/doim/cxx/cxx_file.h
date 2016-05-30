//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <tuple>
#include <variant>

namespace doim
{
class CxxFile;
typedef shared_ptr<CxxFile> CxxFileSPtr;
typedef Set<CxxFile> CxxFileSet;
typedef shared_ptr<CxxFileSet> CxxFileSetSPtr;

struct CxxFileVariants
{
    typedef variant<ProtobufFileSPtr> OriginSPtr;
};

class CxxFile : public CxxFileVariants,
                public Element<CxxFile,
                               FsFileSPtr,
                               CxxIncludeDirectorySetSPtr,
                               CxxFileVariants::OriginSPtr>
{
public:
    CxxFile(const FsFileSPtr& file,
            const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories,
            const OriginSPtr& origin);

    const FsFileSPtr& file() const
    {
        return std::get<0>(mArgs);
    }

    const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories() const
    {
        return std::get<1>(mArgs);
    }

    const OriginSPtr& origin() const
    {
        return std::get<2>(mArgs);
    }
};
}
