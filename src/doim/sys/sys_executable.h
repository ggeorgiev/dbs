//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/fs/fs_relative_file.h"
#include "doim/element.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <variant>

namespace doim
{
class SysExecutable;
typedef shared_ptr<SysExecutable> SysExecutableSPtr;

struct SysExecutableVariants
{
    typedef variant<doim::FsFileSPtr, doim::FsRelativeFileSPtr> ApplicationSPtr;
};

class SysExecutable
    : public SysExecutableVariants,
      public Element<SysExecutable, SysExecutableVariants::ApplicationSPtr>
{
public:
    SysExecutable(const ApplicationSPtr& application);

    const ApplicationSPtr& application() const
    {
        return std::get<0>(mArgs);
    }

    string path(const doim::FsDirectorySPtr& root) const;
};
}
