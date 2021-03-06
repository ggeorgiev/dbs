//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/sys/sys_executable.h"
#include "err/err_assert.h"
#include <boost/hana/for_each.hpp>

namespace doim
{
SysExecutable::SysExecutable(const ApplicationSPtr& application)
    : Element(application)
{
    ASSERT(apply_visitor(vst::isUnique, application));
}

string SysExecutable::path(const doim::FsDirectorySPtr& root) const
{
    if (application().type() == typeid(FsFileSPtr))
        return boost::get<FsFileSPtr>(application())->path(root);

    return boost::get<FsRelativeFileSPtr>(application())->name();
}
}
