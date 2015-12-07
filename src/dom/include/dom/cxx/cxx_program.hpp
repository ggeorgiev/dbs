//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"

#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
class CxxProgram;
typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram
{
public:
    template <typename T>
    std::vector<T> difference(const std::unordered_set<T>& a,
                              const std::unordered_set<T>& b)
    {
        std::vector<T> result;
        std::copy_if(a.begin(), a.end(), back_inserter(result), [&b](T needle) {
            return b.find(needle) == b.end();
        });
        return result;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
        EHEnd;
    }

    ECode updateCxxFiles(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxFiles.swap(files);
        EHEnd;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries()
    {
        return mCxxLibraries;
    }

    const std::unordered_set<doim::FsFileSPtr>& cxxFiles()
    {
        return mCxxFiles;
    }

    // Computations
    std::unordered_set<doim::CxxIncludeDirectorySPtr> cxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        std::unordered_set<doim::CxxIncludeDirectorySPtr> directories;

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->cxxIncludeDirectories(root);
            directories.insert(libDirectories.begin(), libDirectories.end());
        }

        return directories;
    }

    std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles(
        const doim::FsDirectorySPtr& root,
        const doim::FsDirectorySPtr& intermediate) const
    {
        std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles;

        const auto& directories = cxxIncludeDirectories(root);

        for (const auto& cxxFile : mCxxFiles)
        {
            const auto& directory =
                doim::gManager->obtainCorrespondingDirectory(cxxFile->directory(),
                                                             root,
                                                             intermediate);
            const auto& output =
                doim::gManager->obtainFile(directory, cxxFile->name() + ".o");

            auto objectFile =
                std::make_shared<doim::CxxObjectFile>(cxxFile, directories, output);
            objectFile = doim::gManager->unique(objectFile);

            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
    std::unordered_set<doim::FsFileSPtr> mCxxFiles;
};
}
