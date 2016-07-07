//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_framework.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/fs/fs_file.h"
#include "doim/set.hpp"
#include <unordered>

namespace dom
{
ECode CxxProgram::updateCxxLibraries(CxxLibrarySet& libraries)
{
    mCxxLibraries.swap(libraries);
    EHEnd;
}

ECode CxxProgram::updateName(const string& name)
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
    auto objPurpose = objectPurpose(purpose);
    *objectFiles = cxxObjectFiles(objPurpose, root, intermediate);

    auto frameworks = doim::CxxFrameworkSet::make();
    auto staticLibraries = doim::CxxStaticLibrarySet::make();

    auto libraries = recursiveCxxLibraries();

    for (const auto& cxxLibrary : libraries)
    {
        const auto& libFrameworks = cxxLibrary->frameworks();
        frameworks->insert(libFrameworks.begin(), libFrameworks.end());

        if (cxxLibrary->binary() != nullptr)
        {
            auto staticLibrary = doim::CxxStaticLibrary::unique(cxxLibrary->binary());
            staticLibraries->insert(staticLibrary);
        }
        else
        {
            const auto& libObjectFiles =
                cxxLibrary->cxxObjectFiles(objPurpose, root, intermediate);
            objectFiles->insert(libObjectFiles.begin(), libObjectFiles.end());
        }
    }
    objectFiles = doim::CxxObjectFileSet::unique(objectFiles);

    frameworks = doim::CxxFrameworkSet::unique(frameworks);
    staticLibraries = doim::CxxStaticLibrarySet::unique(staticLibraries);

    const auto& programFile = doim::FsFile::obtain(intermediate, name());
    return doim::CxxProgram::unique(
        purpose, programFile, staticLibraries, frameworks, objectFiles);
}

CxxLibrarySet CxxProgram::recursiveCxxLibraries() const
{
    CxxLibrarySet libraries = mCxxLibraries;
    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libs = cxxLibrary->recursiveCxxLibraries();
        libraries.insert(libs.begin(), libs.end());
    }
    return libraries;
}

doim::CxxIncludeDirectorySetSPtr CxxProgram::visibleCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto directories = doim::CxxIncludeDirectorySet::make();

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libDirectories =
            cxxLibrary->recursivePublicCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());
    }

    return doim::CxxIncludeDirectorySet::unique(directories);
}
}
