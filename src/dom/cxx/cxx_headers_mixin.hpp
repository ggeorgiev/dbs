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

    ECode updateCxxHeaders(doim::CxxHeader::EVisibility visibility,
                           const doim::FsDirectorySPtr& directory,
                           doim::FsFileSet& files)
    {
        auto it = mHeaders.find(visibility);
        if (it == mHeaders.end())
            it = mHeaders.insert({visibility, doim::FsDirectory::FsFileSetMap()}).first;

        auto& headerSet = it->second;
        auto itd = headerSet.find(directory);
        if (itd == headerSet.end())
            itd = headerSet.insert({directory, files}).first;

        EHEnd;
    }

    // Computations

    typedef unordered_map<doim::FsFileSPtr, doim::CxxSourceOriginSPtr> FileOriginMap;

    doim::CxxIncludeDirectorySet cxxIncludeDirectories(
        doim::CxxHeader::EVisibility visibility, const doim::FsDirectorySPtr& root) const
    {
        auto self = static_cast<const Subject*>(this);

        doim::CxxIncludeDirectorySet set;

        const auto& it = mHeaders.find(visibility);

        doim::FsDirectorySet directories;
        if (it != mHeaders.end())
            for (const auto& headerSet : it->second)
                directories.insert(headerSet.first);

        const auto& protobufs = self->protobufs();
        const auto& protobufIt = protobufs.find(visibility);
        if (protobufIt != protobufs.end())
            for (const auto& headerSet : protobufIt->second)
                directories.insert(headerSet.first);

        for (const auto& directory : directories)
        {
            FileOriginMap files;

            if (it != mHeaders.end())
            {
                const auto& fileSetIt = it->second.find(directory);
                if (fileSetIt != it->second.end())
                    for (const auto& file : fileSetIt->second)
                        files.insert({file, nullptr});
            }

            if (protobufIt != protobufs.end())
            {
                const auto& protobufFileSetIt = protobufIt->second.find(directory);
                if (protobufFileSetIt != protobufIt->second.end())
                    for (const auto& file : protobufFileSetIt->second)
                        files.insert({self->headerFile(file),
                                      doim::ProtobufFile::unique(directory, file)});
            }

            const auto& headers = cxxHeaders(visibility, root, files);

            set.insert(doim::CxxIncludeDirectory::unique(self->cxxIncludeDirectoryType(),
                                                         directory,
                                                         headers));
        }

        return set;
    }

    doim::CxxHeaderSetSPtr cxxHeaders(doim::CxxHeader::EVisibility visibility,
                                      const doim::FsDirectorySPtr& root,
                                      const FileOriginMap& files) const
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
            const auto& cxxHeader = doim::CxxHeader::unique(
                type, visibility, header.first, directories, header.second);
            headers->insert(cxxHeader);
        }
        return doim::CxxHeaderSet::unique(headers);
    }

private:
    doim::CxxHeader::VisibilityFsDirectoryFsFileSetMapMap mHeaders;
};
}
