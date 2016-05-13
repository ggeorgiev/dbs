//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "err/err.h"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class CxxPrivateHeadersMixin
{
public:
    typedef T Subject;

    ECode updateCxxPrivateHeaders(const doim::FsDirectorySPtr& directory,
                                  std::unordered_set<doim::FsFileSPtr>& files)
    {
        mPrivateHeadersDirectory = directory;
        mCxxPrivateHeaders = std::make_shared<doim::FsFileSet>();
        mCxxPrivateHeaders->swap(files);
        EHEnd;
    }

    doim::FsDirectorySPtr privateHeadersDirectory() const
    {
        return mPrivateHeadersDirectory;
    }

    const doim::FsFileSetSPtr privateHeaders() const
    {
        return mCxxPrivateHeaders;
    }

    // Computations
    doim::CxxIncludeDirectorySPtr cxxPrivateIncludeDirectory(
        const doim::FsDirectorySPtr& root) const
    {
        if (privateHeadersDirectory() == nullptr)
            return nullptr;

        const auto& directory =
            doim::CxxIncludeDirectory::make(static_cast<const Subject*>(this)
                                                ->cxxIncludeDirectoryType(),
                                            privateHeadersDirectory(),
                                            privateCxxHeaders(root));
        return doim::gManager->unique(directory);
    }

    doim::CxxHeaderSetSPtr privateCxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();

        if (mCxxPrivateHeaders != nullptr)
        {
            auto type = static_cast<const Subject*>(this)->cxxHeaderType();
            const auto& directories =
                static_cast<const Subject*>(this)->indirectCxxIncludeDirectories(root);
            for (const auto& header : *mCxxPrivateHeaders)
            {
                const auto& cxxHeader = doim::CxxHeader::make(type, header, directories);
                headers->insert(doim::gManager->unique(cxxHeader));
            }
        }
        return doim::gManager->unique(headers);
    }

private:
    doim::FsDirectorySPtr mPrivateHeadersDirectory;
    doim::FsFileSetSPtr mCxxPrivateHeaders;
};
}
