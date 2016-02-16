//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_library.hpp"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
class CxxProgram;
typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram : public CxxFilesMixin<CxxProgram>,
                   public CxxPrivateHeadersMixin<CxxProgram>
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

    ECode updateName(const std::string& name)
    {
        mName = name;
        EHEnd;
    }

    std::string name() const
    {
        return mName;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
        EHEnd;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    // Computations
    doim::CxxProgramSPtr cxxProgram(const doim::FsDirectorySPtr& root,
                                    const doim::FsDirectorySPtr& intermediate) const
    {
        auto objectFiles = std::make_shared<doim::CxxObjectFileSet>();
        *objectFiles = cxxObjectFiles(root, intermediate);

        for (const auto& cxxLibrary : recursiveCxxLibraries())
        {
            if (cxxLibrary->binary() != nullptr)
                continue;

            const auto& libObjectFiles = cxxLibrary->cxxObjectFiles(root, intermediate);
            objectFiles->insert(libObjectFiles.begin(), libObjectFiles.end());
        }

        const auto& outputFile = doim::gManager->obtainFile(intermediate, name());
        objectFiles = doim::gManager->unique(objectFiles);

        auto program = std::make_shared<doim::CxxProgram>(outputFile, objectFiles);
        return doim::gManager->unique(program);
    }

    std::unordered_set<CxxLibrarySPtr> recursiveCxxLibraries() const
    {
        std::unordered_set<CxxLibrarySPtr> libraries = mCxxLibraries;
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libs = cxxLibrary->recursiveCxxLibraries();
            libraries.insert(libs.begin(), libs.end());
        }
        return libraries;
    }

    doim::CxxIncludeDirectorySetSPtr recursiveCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

    doim::CxxHeaderSetSPtr cxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
            headers->insert(libHeaders->begin(), libHeaders->end());
        }
        return doim::gManager->unique(headers);
    }

private:
    std::string mName;
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
