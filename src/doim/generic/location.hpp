//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"

namespace doim
{
/*
 * Generic Location
 *
 * For sake of simplicity and code efficiency the generic location is the shared with the
 * fs directory infrastructure.
 *
 */

typedef FsDirectory Location;
typedef FsDirectorySPtr LocationSPtr;
typedef FsDirectorySet LocationSet;
typedef FsDirectorySetSPtr LocationSetSPtr;
}
