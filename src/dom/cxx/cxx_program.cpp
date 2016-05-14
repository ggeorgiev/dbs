//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/manager.h"
#include <unordered_set>

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

static doim::CxxObjectFile::EPurpose objectPurpose(doim::CxxProgram::EPurpose purpose)
{
    switch (purpose)
    {
        case doim::CxxProgram::EPurpose::kDebug:
            return doim::CxxObjectFile::EPurpose::kDebug;
        case doim::CxxProgram::EPurpose::kRelease:
            return doim::CxxObjectFile::EPurpose::kRelease;
        case doim::CxxProgram::EPurpose::kProfile:
            return doim::CxxObjectFile::EPurpose::kProfile;
    }
}

doim::CxxProgramSPtr CxxProgram::cxxProgram(
    doim::CxxProgram::EPurpose purpose,
    const doim::FsDirectorySPtr& root,
    const doim::FsDirectorySPtr& intermediate) const
{
    auto objectFiles = doim::CxxObjectFileSet::make();
    *objectFiles = cxxObjectFiles(objectPurpose(purpose), root, intermediate);

    auto staticLibraries = doim::CxxStaticLibrarySet::make();

    auto libraries = recursiveCxxLibraries();

    for (const auto& cxxLibrary : libraries)
    {
        if (cxxLibrary->binary() != nullptr)
        {
            auto staticLibrary = doim::gManager->unique(
                doim::CxxStaticLibrary::make(cxxLibrary->binary()));
            staticLibraries->insert(staticLibrary);
        }
        else
        {
            const auto& libObjectFiles =
                cxxLibrary->cxxObjectFiles(objectPurpose(purpose), root, intermediate);
            objectFiles->insert(libObjectFiles.begin(), libObjectFiles.end());
        }
    }
    objectFiles = doim::gManager->unique(objectFiles);
    staticLibraries = doim::gManager->unique(staticLibraries);

    const auto& programFile = doim::gManager->obtainFile(intermediate, name());
    auto program =
        doim::CxxProgram::make(purpose, programFile, staticLibraries, objectFiles);
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
    auto directories = doim::CxxIncludeDirectorySet::make();

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());
    }

    return doim::gManager->unique(directories);
}

doim::CxxHeaderSetSPtr CxxProgram::cxxHeaders(const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();
    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
        headers->insert(libHeaders->begin(), libHeaders->end());
    }
    return doim::gManager->unique(headers);
}
}
