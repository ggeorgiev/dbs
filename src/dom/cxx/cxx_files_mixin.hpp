//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
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

        doim::CxxFileSet cxxFiles;

        const auto& directories =
            static_cast<const Subject*>(this)->recursiveCxxIncludeDirectories(root);

        for (const auto& directory : *directories)
            DLOG("include directory: {0}", directory->directory()->path(root));

        for (const auto& fsFile : mCxxFilesList)
        {
            auto cxxFile = doim::CxxFile::unique(fsFile, directories);
            cxxFiles.insert(cxxFile);

            DLOG("fsFile: {0}", fsFile->path());
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
                doim::gManager->obtainFile(directory, cxxFile->file()->name() + ".o");

            auto objectFile = doim::CxxObjectFile::unique(purpose, cxxFile, outputFile);
            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    doim::FsFileSet mCxxFilesList;
};
}
