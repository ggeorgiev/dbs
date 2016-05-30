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

    ECode updateProtobufsList(doim::FsFileSet& files)
    {
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

    doim::ProtobufFileSet protobufFiles(const doim::FsDirectorySPtr& root) const
    {
        TLOG_FUNCTION;

        doim::ProtobufFileSet protobufFiles;

        for (const auto& fsFile : mProtobufsList)
        {
            auto protobufFile = doim::ProtobufFile::unique(fsFile);
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

        auto cxxIncludeDirectories = doim::CxxIncludeDirectorySet::make();
        cxxIncludeDirectories->insert(protobuflib);
        cxxIncludeDirectories =
            doim::CxxIncludeDirectorySet::unique(cxxIncludeDirectories);

        for (const auto& protobufFile : files)
        {
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
    doim::FsFileSet mProtobufsList;
};
}
