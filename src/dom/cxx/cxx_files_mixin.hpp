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
class CxxFilesMixin
{
public:
    typedef T Subject;

    ECode updateCxxFilesList(doim::FsFileSet& files)
    {
        mCxxFilesList.swap(files);
        EHEnd;
    }

    const doim::FsFileSet& cxxFilesList() const
    {
        return mCxxFilesList;
    }

    doim::CxxFileSet cxxFiles(const doim::FsDirectorySPtr& root) const
    {
        TLOG_FUNCTION;

        auto self = static_cast<const Subject*>(this);

        const auto& directories = self->visibleCxxIncludeDirectories(root);

        doim::CxxFileSet cxxFiles;

        const auto& protobufCxxFiles = self->protobufCxxFiles(root);
        cxxFiles.insert(protobufCxxFiles.begin(), protobufCxxFiles.end());

        for (const auto& fsFile : mCxxFilesList)
        {
            auto cxxFile = doim::CxxFile::unique(fsFile, directories, nullptr);
            cxxFiles.insert(cxxFile);

            DLOG("generate cxx file for: {}", fsFile->path(root));
        }

        return cxxFiles;
    }

    doim::CxxObjectFileSet cxxObjectFiles(doim::CxxObjectFile::EPurpose purpose,
                                          const doim::FsDirectorySPtr& root,
                                          const doim::FsDirectorySPtr& intermediate) const
    {
        doim::CxxObjectFileSet cxxObjectFiles;

        const auto& files = cxxFiles(root);

        for (const auto& cxxFile : files)
        {
            const auto& directory =
                doim::FsDirectory::corresponding(cxxFile->file()->directory(),
                                                 root,
                                                 intermediate);
            const auto& outputFile =
                doim::FsFile::obtain(directory, cxxFile->file()->name() + ".o");

            auto objectFile = doim::CxxObjectFile::unique(purpose, cxxFile, outputFile);
            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    doim::FsFileSet mCxxFilesList;
};
}
