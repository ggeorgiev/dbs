//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.h"
#include "err/err.h"

namespace parser
{
class DbsParser
{
public:
    ECode parse(const doim::FsFileSPtr& dbsFile);
};
}
