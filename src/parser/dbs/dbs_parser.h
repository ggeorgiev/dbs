//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <str>
#include <vector>

namespace parser
{
class DbsParser
{
public:
    ECode parse(const doim::FsFileSPtr& dbsFile);
};
}
