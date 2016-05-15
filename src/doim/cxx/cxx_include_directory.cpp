//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_header.h"
#include "doim/manager.h"
#include "err/err_assert.h"
#include <unordered>
#include <utility>

namespace doim
{
CxxIncludeDirectory::CxxIncludeDirectory(const EType type,
                                         const FsDirectorySPtr& directory,
                                         const CxxHeaderSetSPtr& headerFiles)
    : Base(type, directory, headerFiles)
{
    ASSERT(directory != nullptr);
    ASSERT(directory->isUnique());
    ASSERT(headerFiles->isUnique());
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

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory)
{
    out << "type:";
    switch (directory.type())
    {
        case CxxIncludeDirectory::EType::kUser:
            out << "user";
            break;
        case CxxIncludeDirectory::EType::kSystem:
            out << "system";
            break;
    }
    out << ", directory: " << directory.directory()->path() << std::endl;
    for (const auto& header : *directory.headerFiles())
        out << *header << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories)
{
    for (const auto& directory : directories)
        out << *directory << std::endl;
    return out;
}
}
