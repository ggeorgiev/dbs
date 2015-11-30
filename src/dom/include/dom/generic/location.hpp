//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_directory.hpp"

namespace dom
{
/*
 * Generic Location
 *
 * For sake of simplicity and code efficiency the generic location is the shared with the fs
 * directory infrastructure.
 *
 */

typedef FsDirectory Location;
typedef FsDirectorySPtr LocationSPtr;
}
