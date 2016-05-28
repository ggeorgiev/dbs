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
    doim::CxxIncludeDirectorySPtr cxxPrivateIncludeDirectory(
        const doim::FsDirectorySPtr& root) const
    {
        const auto& directory = headersDirectory(doim::CxxHeader::EVisibility::kPrivate);
        if (directory == nullptr)
            return nullptr;

        return doim::CxxIncludeDirectory::unique(
            static_cast<const Subject*>(this)->cxxIncludeDirectoryType(),
            directory,
            privateCxxHeaders(root));
    }

    doim::CxxHeaderSetSPtr privateCxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = doim::CxxHeaderSet::make();

        const auto files = headerFiles(doim::CxxHeader::EVisibility::kPrivate);

        if (files == nullptr)
            return doim::CxxHeaderSet::unique(headers);

        auto type = static_cast<const Subject*>(this)->cxxHeaderType();
        const auto& directories =
            static_cast<const Subject*>(this)->indirectCxxIncludeDirectories(root);
        for (const auto& header : *files)
        {
            const auto& cxxHeader = doim::CxxHeader::unique(
                type, doim::CxxHeader::EVisibility::kPrivate, header, directories);
            headers->insert(cxxHeader);
        }
        return doim::CxxHeaderSet::unique(headers);
    }

private:
    std::map<doim::CxxHeader::EVisibility, Headers> mHeaders;
};
}
