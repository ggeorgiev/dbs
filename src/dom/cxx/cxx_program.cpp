//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"

namespace dom
{
ECode CxxProgram::updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
{
    mCxxLibraries.swap(libraries);
    EHEnd;
}

ECode CxxProgram::updateName(const std::string& name)
{
    mName = name;
    EHEnd;
}

doim::CxxProgramSPtr CxxProgram::cxxProgram(
    const doim::FsDirectorySPtr& root, const doim::FsDirectorySPtr& intermediate) const
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

std::unordered_set<CxxLibrarySPtr> CxxProgram::recursiveCxxLibraries() const
{
    std::unordered_set<CxxLibrarySPtr> libraries = mCxxLibraries;
    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libs = cxxLibrary->recursiveCxxLibraries();
        libraries.insert(libs.begin(), libs.end());
    }
    return libraries;
}

doim::CxxIncludeDirectorySetSPtr CxxProgram::recursiveCxxIncludeDirectories(
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

doim::CxxHeaderSetSPtr CxxProgram::cxxHeaders(const doim::FsDirectorySPtr& root) const
{
    auto headers = std::make_shared<doim::CxxHeaderSet>();
    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
        headers->insert(libHeaders->begin(), libHeaders->end());
    }
    return doim::gManager->unique(headers);
}
}
