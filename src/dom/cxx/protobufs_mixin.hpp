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

    ECode updateProtobufsList(doim::FsDirectorySPtr& directory, doim::FsFileSet& files)
    {
        mProtobufsDirectory = directory;
        mProtobufsList.swap(files);
        EHEnd;
    }

    const doim::FsFileSet& protobufsList() const
    {
        return mProtobufsList;
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

    const doim::CxxHeaderSPtr headerFile(
        const doim::CxxHeader::EType type,
        const doim::ProtobufFileSPtr& protobufFile,
        const doim::CxxIncludeDirectorySetSPtr& cxxIncludeDirectories) const
    {
        auto name = protobufFile->file()->name();

        const auto& fsfile =
            doim::FsFile::obtain(protobufFile->file()->directory(),
                                 name.substr(0, name.length() - sizeof("proto")) +
                                     ".pb.h");

        return doim::CxxHeader::unique(type,
                                       doim::CxxHeader::EVisibility::kPublic,
                                       fsfile,
                                       cxxIncludeDirectories,
                                       protobufFile);
    }

    doim::ProtobufFileSet protobufFiles(const doim::FsDirectorySPtr& root) const
    {
        TLOG_FUNCTION;

        doim::ProtobufFileSet protobufFiles;

        for (const auto& fsFile : mProtobufsList)
        {
            auto protobufFile = doim::ProtobufFile::unique(mProtobufsDirectory, fsFile);
            protobufFiles.insert(protobufFile);

            DLOG("generate protobuf file for: {0}", fsFile->path());
        }

        return protobufFiles;
    }

    doim::CxxObjectFileSet protobufCxxObjectFiles(
        doim::CxxObjectFile::EPurpose purpose,
        const doim::FsDirectorySPtr& root,
        const doim::FsDirectorySPtr& intermediate) const
    {
        doim::CxxObjectFileSet cxxObjectFiles;

        const auto& files = protobufFiles(root);

        auto protobuflib = doim::CxxIncludeDirectory::unique(
            doim::CxxIncludeDirectory::EType::kSystem,
            doim::FsDirectory::obtain(root, "protobuf/include"),
            doim::CxxHeaderSet::unique(doim::CxxHeaderSet::make()));

        auto headers = doim::CxxHeaderSet::make();
        for (const auto& protobufFile : files)
        {
            auto cxxIncludeDirectories = doim::CxxIncludeDirectorySet::make();
            cxxIncludeDirectories->insert(protobuflib);
            cxxIncludeDirectories =
                doim::CxxIncludeDirectorySet::unique(cxxIncludeDirectories);

            const auto& directory =
                doim::FsDirectory::corresponding(protobufFile->file()->directory(),
                                                 root,
                                                 intermediate);
            auto header = headerFile(doim::CxxHeader::EType::kUser,
                                     protobufFile,
                                     cxxIncludeDirectories);
            headers->insert(header);
        }

        headers = doim::CxxHeaderSet::unique(headers);

        for (const auto& protobufFile : files)
        {
            auto protobufGen = doim::CxxIncludeDirectory::unique(
                doim::CxxIncludeDirectory::EType::kSystem,
                protobufFile->directory(),
                headers);

            auto cxxIncludeDirectories = doim::CxxIncludeDirectorySet::make();
            cxxIncludeDirectories->insert(protobuflib);
            cxxIncludeDirectories->insert(protobufGen);
            cxxIncludeDirectories =
                doim::CxxIncludeDirectorySet::unique(cxxIncludeDirectories);

            const auto& directory =
                doim::FsDirectory::corresponding(protobufFile->file()->directory(),
                                                 root,
                                                 intermediate);
            const auto& outputFile =
                doim::FsFile::obtain(directory, protobufFile->file()->name() + ".o");

            auto compilefile = cxxFile(protobufFile, cxxIncludeDirectories);

            auto objectFile =
                doim::CxxObjectFile::unique(purpose, compilefile, outputFile);
            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    doim::FsDirectorySPtr mProtobufsDirectory;
    doim::FsFileSet mProtobufsList;
};
}
