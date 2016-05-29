//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
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

struct CxxObjectFileVariants
{
    typedef boost::variant<CxxFileSPtr, ProtobufFileSPtr> SourceSPtr;
};

class CxxObjectFile : public CxxObjectFileEnums,
                      public CxxObjectFileVariants,
                      public Element<CxxObjectFile,
                                     CxxObjectFileEnums::EPurpose,
                                     CxxObjectFileVariants::SourceSPtr,
                                     FsFileSPtr>
{
public:
    CxxObjectFile(EPurpose purpose, const SourceSPtr& source, const FsFileSPtr& file);

    EPurpose purpose() const
    {
        return static_cast<EPurpose>(std::get<0>(mArgs));
    }

    const SourceSPtr& source()
    {
        return std::get<1>(mArgs);
    }

    const CxxFileSPtr& cxxFile()
    {
        return boost::get<CxxFileSPtr>(source());
    }

    const FsFileSPtr& file()
    {
        return std::get<2>(mArgs);
    }
};
}
