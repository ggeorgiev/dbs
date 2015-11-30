//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"

#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace dom
{
class CppLibrary
{
public:
    ECode updateAttribute(const Attribute& attribute) { EHEnd; }
    ECode updateBinary(const FsFileSPtr& binary)
    {
        mBinary = binary;
        EHEnd;
    }
    ECode updatePublicHeadersDirectory(const FsDirectorySPtr& publicHeaders)
    {
        mPublicHeaders = publicHeaders;
        EHEnd;
    }

    FsDirectorySPtr publicHeadersDirectory() { return mPublicHeaders; }
private:
    FsFileSPtr mBinary;
    FsDirectorySPtr mPublicHeaders;
};

typedef std::shared_ptr<CppLibrary> CppLibrarySPtr;
}
