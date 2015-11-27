//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <memory>

namespace dom
{
class FsDirectory
{
public:
private:
    FdDirectorySPtr mParent;
};

typedef std::shared_ptr<FsDirectory> FsDirectorySPtr;
}
