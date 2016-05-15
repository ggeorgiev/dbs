//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/element.hpp"
#include "doim/fs/fs_file.h"
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered>

namespace doim
{
class CxxProgram;
typedef shared_ptr<CxxProgram> CxxProgramSPtr;
typedef Set<CxxProgram> CxxProgramSet;
typedef shared_ptr<CxxProgramSet> CxxProgramSetSPtr;

struct CxxProgramEnums
{
    enum class EPurpose
    {
        kDebug,
        kRelease,
        kProfile,
    };
};

class CxxProgram : public CxxProgramEnums,
                   public Element<CxxProgram,
                                  CxxProgramEnums::EPurpose,
                                  FsFileSPtr,
                                  CxxStaticLibrarySetSPtr,
                                  CxxObjectFileSetSPtr>
{
public:
    CxxProgram(EPurpose purpose,
               const FsFileSPtr& file,
               const CxxStaticLibrarySetSPtr& staticLibraries,
               const CxxObjectFileSetSPtr& cxxObjectFiles);

    EPurpose purpose() const
    {
        return static_cast<EPurpose>(std::get<0>(mArgs));
    }

    const FsFileSPtr& file() const
    {
        return std::get<1>(mArgs);
    }

    const CxxStaticLibrarySetSPtr& staticLibraries()
    {
        return std::get<2>(mArgs);
    }

    const CxxObjectFileSetSPtr& cxxObjectFiles()
    {
        return std::get<3>(mArgs);
    }
};
}
