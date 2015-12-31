//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"

#include "doim/manager.h"
#include "doim/cxx/cxx_file.hpp"
#include "doim/cxx/cxx_object_file.hpp"
#include "doim/fs/fs_file.hpp"

#include "err/err.h"

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

    ECode updateCxxFilesList(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxFilesList.swap(files);
        EHEnd;
    }

    const std::unordered_set<doim::FsFileSPtr>& cxxFilesList() const
    {
        return mCxxFilesList;
    }

    std::unordered_set<doim::CxxFileSPtr> cxxFiles(
        const doim::FsDirectorySPtr& root) const
    {
        std::unordered_set<doim::CxxFileSPtr> cxxFiles;

        const auto& directories =
            static_cast<const Subject*>(this)->cxxIncludeDirectories(root);
        const auto& headers = static_cast<const Subject*>(this)->cxxHeaders(root);

        for (const auto& fsFile : mCxxFilesList)
        {
            auto cxxFile = std::make_shared<doim::CxxFile>(fsFile, directories, headers);
            cxxFiles.insert(doim::gManager->unique(cxxFile));
        }

        return cxxFiles;
    }

    std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles(
        const doim::FsDirectorySPtr& root,
        const doim::FsDirectorySPtr& intermediate) const
    {
        std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles;

        const auto& files = cxxFiles(root);

        for (const auto& cxxFile : files)
        {
            const auto& directory =
                doim::gManager->obtainCorrespondingDirectory(cxxFile->file()->directory(),
                                                             root,
                                                             intermediate);
            const auto& outputFile =
                doim::gManager->obtainFile(directory, cxxFile->file()->name() + ".o");

            auto objectFile = std::make_shared<doim::CxxObjectFile>(cxxFile, outputFile);
            objectFile = doim::gManager->unique(objectFile);

            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    std::unordered_set<doim::FsFileSPtr> mCxxFilesList;
};
}
