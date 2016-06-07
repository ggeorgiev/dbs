//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "doim/protobuf/protobuf_file.h"
#include "err/err.h"
#include "log/log.h"
#include "log/log_function.hpp"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class ProtobufsMixin
{
public:
    typedef T Subject;

    ECode updateProtobufsList(doim::CxxHeader::EVisibility visibility,
                              doim::FsDirectorySPtr& directory,
                              doim::FsFileSet& files)
    {
        auto it = mProtobufs.find(visibility);
        if (it == mProtobufs.end())
            it = mProtobufs.insert({visibility, doim::FsDirectory::FsFileSetMap()}).first;

        auto& headerSet = it->second;
        auto itd = headerSet.find(directory);
        if (itd == headerSet.end())
            itd = headerSet.insert({directory, files}).first;

        EHEnd;
    }

    const doim::CxxHeader::VisibilityFsDirectoryFsFileSetMapMap protobufs() const
    {
        return mProtobufs;
    }

    const doim::FsFileSPtr headerFile(const doim::FsFileSPtr& file) const
    {
        auto name = file->name();

        return doim::FsFile::obtain(file->directory(),
                                    name.substr(0, name.length() - sizeof("proto")) +
                                        ".pb.h");
    }

    const doim::CxxFileSPtr cxxFile(
        const doim::ProtobufFileSPtr& protobufFile,
        const doim::CxxIncludeDirectorySetSPtr& cxxIncludeDirectories) const
    {
        auto name = protobufFile->file()->name();

        const auto& fsfile =
            doim::FsFile::obtain(protobufFile->file()->directory(),
                                 name.substr(0, name.length() - sizeof("proto")) +
                                     ".pb.cc");

        return doim::CxxFile::unique(fsfile, cxxIncludeDirectories, protobufFile);
    }

    doim::CxxFileSet protobufCxxFiles(const doim::FsDirectorySPtr& root) const
    {
        auto self = static_cast<const Subject*>(this);

        const auto& directories = self->visibleCxxIncludeDirectories(root);

        doim::CxxFileSet cxxFiles;

        for (const auto& visibilityIt : mProtobufs)
        {
            for (const auto& directoryIt : visibilityIt.second)
            {
                for (const auto& file : directoryIt.second)
                {
                    auto protobufFile =
                        doim::ProtobufFile::unique(directoryIt.first, file);
                    cxxFiles.insert(cxxFile(protobufFile, directories));
                }
            }
        }
        return cxxFiles;
    }

private:
    doim::CxxHeader::VisibilityFsDirectoryFsFileSetMapMap mProtobufs;
};
}
