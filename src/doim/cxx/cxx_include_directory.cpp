//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/manager.h"
#include "err/err_assert.h"

namespace doim
{
CxxIncludeDirectory::CxxIncludeDirectory(const Type type,
                                         const FsDirectorySPtr& directory,
                                         const CxxHeaderSetSPtr& headerFiles)
    : Base(static_cast<int>(type), directory, headerFiles)
{
    ASSERT(directory != nullptr);
    ASSERT(gManager->isUnique(directory));
    ASSERT(gManager->isUnique(headerFiles));
}

void CxxIncludeDirectory::finally()
{
    for (const auto& header : *headerFiles())
    {
        ASSERT(mFiles.count(header->file()) == 0);
        mFiles[header->file()] = header;
    }
}

CxxHeaderSPtr CxxIncludeDirectory::header(const FsFileSPtr& file) const
{
    const auto& it = mFiles.find(file);
    if (it != mFiles.end())
        return it->second;
    return nullptr;
}
}
