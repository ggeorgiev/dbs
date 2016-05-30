//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
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

        for (const auto& protobufFile : files)
        {
            /*
                        const auto& directory =
                            doim::FsDirectory::corresponding(protobufFile->file()->directory(),
                                                             root,
                                                             intermediate);
                        const auto& outputFile =
                            doim::FsFile::obtain(directory, protobufFile->file()->name() +
               ".o");

                        auto objectFile =
                            doim::CxxObjectFile::unique(purpose, protobufFile,
               outputFile);
                        cxxObjectFiles.insert(objectFile);
            */
        }
        return cxxObjectFiles;
    }

private:
    doim::FsFileSet mProtobufsList;
};
}
