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

    typedef unordered_map<doim::FsDirectorySPtr, doim::FsFileSet> HeaderSet;
    typedef unordered_map<doim::CxxHeader::EVisibility, HeaderSet> VisibilitySet;

    ECode updateCxxHeaders(doim::CxxHeader::EVisibility visibility,
                           const doim::FsDirectorySPtr& directory,
                           doim::FsFileSet& files)
    {
        auto it = mHeaders.find(visibility);
        if (it == mHeaders.end())
            it = mHeaders.insert({visibility, HeaderSet()}).first;

        auto& headerSet = it->second;
        auto itd = headerSet.find(directory);
        if (itd == headerSet.end())
            itd = headerSet.insert({directory, files}).first;

        EHEnd;
    }

    // Computations
    doim::CxxIncludeDirectorySet cxxIncludeDirectories(
        doim::CxxHeader::EVisibility visibility, const doim::FsDirectorySPtr& root) const
    {
        auto self = static_cast<const Subject*>(this);

        doim::CxxIncludeDirectorySet set;

        const auto& it = mHeaders.find(visibility);
        if (it == mHeaders.end())
            return set;

        for (const auto& headerSet : it->second)
        {
            const auto& headers = cxxHeaders(visibility, root, headerSet.second);
            set.insert(doim::CxxIncludeDirectory::unique(self->cxxIncludeDirectoryType(),
                                                         headerSet.first,
                                                         headers));
        }

        return set;
    }

    doim::CxxHeaderSetSPtr cxxHeaders(doim::CxxHeader::EVisibility visibility,
                                      const doim::FsDirectorySPtr& root,
                                      const doim::FsFileSet& files) const
    {
        auto self = static_cast<const Subject*>(this);

        auto headers = doim::CxxHeaderSet::make();

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

        for (const auto& header : files)
        {
            const auto& cxxHeader =
                doim::CxxHeader::unique(type, visibility, header, directories, nullptr);
            headers->insert(cxxHeader);
        }
        return doim::CxxHeaderSet::unique(headers);
    }

private:
    VisibilitySet mHeaders;
};
}
