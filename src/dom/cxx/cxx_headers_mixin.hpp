//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"

#include "err/err.h"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class CxxHeadersMixin
{
public:
    typedef T Subject;

    struct Headers
    {
        doim::FsDirectorySPtr mDirectory;
        doim::FsFileSetSPtr mFiles;
    };

    ECode updateCxxHeaders(doim::CxxHeader::EVisibility visibility,
                           const doim::FsDirectorySPtr& directory,
                           doim::FsFileSet& files)
    {
        mHeaders[visibility] = {directory, doim::FsFileSet::make(files)};
        EHEnd;
    }

    doim::FsDirectorySPtr headersDirectory(doim::CxxHeader::EVisibility visibility) const
    {
        const auto it = mHeaders.find(visibility);
        if (it == mHeaders.end())
            return nullptr;
        return it->second.mDirectory;
    }

    const doim::FsFileSetSPtr headerFiles(doim::CxxHeader::EVisibility visibility) const
    {
        const auto it = mHeaders.find(visibility);
        if (it == mHeaders.end())
            return nullptr;
        return it->second.mFiles;
    }

    // Computations
    doim::CxxIncludeDirectorySet cxxIncludeDirectories(
        doim::CxxHeader::EVisibility visibility, const doim::FsDirectorySPtr& root) const
    {
        auto self = static_cast<const Subject*>(this);

        doim::CxxIncludeDirectorySet set;

        const auto& directory = headersDirectory(visibility);
        if (directory != nullptr)
        {
            set.insert(doim::CxxIncludeDirectory::unique(self->cxxIncludeDirectoryType(),
                                                         directory,
                                                         cxxHeaders(visibility, root)));
        }

        return set;
    }

    doim::CxxHeaderSetSPtr cxxHeaders(doim::CxxHeader::EVisibility visibility,
                                      const doim::FsDirectorySPtr& root) const
    {
        auto self = static_cast<const Subject*>(this);

        auto headers = doim::CxxHeaderSet::make();

        const auto files = headerFiles(visibility);

        if (files == nullptr)
            return doim::CxxHeaderSet::unique(headers);

        auto type = self->cxxHeaderType();

        doim::CxxIncludeDirectorySetSPtr directories;
        switch (visibility)
        {
            case doim::CxxHeader::EVisibility::kPublic:
                directories = self->recursiveProtectedCxxIncludeDirectories(root);
                break;
            case doim::CxxHeader::EVisibility::kProtected:
                directories = self->indirectPublicCxxIncludeDirectories(root);
                break;
            case doim::CxxHeader::EVisibility::kPrivate:
                directories = self->recursivePublicCxxIncludeDirectories(root);
                break;
        }

        for (const auto& header : *files)
        {
            const auto& cxxHeader =
                doim::CxxHeader::unique(type, visibility, header, directories, nullptr);
            headers->insert(cxxHeader);
        }
        return doim::CxxHeaderSet::unique(headers);
    }

private:
    std::map<doim::CxxHeader::EVisibility, Headers> mHeaders;
};
}
