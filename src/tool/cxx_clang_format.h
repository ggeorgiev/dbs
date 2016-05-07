//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_directory.h"
#include "doim/sys/executable.hpp"
#include <memory>

namespace tool
{
class CxxClangFormat;
typedef std::shared_ptr<CxxClangFormat> CxxClangFormatSPtr;

class CxxClangFormat : public std::enable_shared_from_this<CxxClangFormat>
{
public:
    CxxClangFormat(const doim::SysExecutableSPtr& formatter);

    tpool::TaskSPtr formatCommand(const doim::FsDirectorySPtr& directory,
                                  const doim::CxxFileSPtr& cxxFile) const;

private:
    doim::SysExecutableSPtr mFormatter;
};
}
